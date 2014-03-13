#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#include "tables.h"

/*
 * the auxiliary data struct for sorting
 * 
 */
typedef struct {
    int year; 
    int month;
    int day;
    char filename[TEXT_LONG];
} senddate_sort_t;

/*
 * the auxiliary compare function
 * 
 */
int compare_date(const void *a, const void *b) 
{
    senddate_sort_t *ta = (senddate_sort_t *)(a);
    senddate_sort_t *tb = (senddate_sort_t *)(b);
    
    if (ta->year != tb->year) {
        return ta->year > tb->year;
    } else if (ta->month != tb->month) {
        return ta->month > tb->month;
    } 

    return ta->day > tb->day;
        
}

int main(int argc, char **argv)
{
    int i;
    struct stat st;
    
    /* print usage if needed */
    if (argc != 2) {
        fprintf(stderr, "Usage: %s total_record_number\n", argv[0]);
        exit(0);
    }
    
    /* get total record number */
    int total_record_number = atoi(argv[1]);
    
    char filename[TEXT_LONG];
    FILE *fp = NULL;
    
    struct timeval time_start, time_end;
    
    /* start time */
    gettimeofday(&time_start, NULL);
    
    /* create the auxiliary data struct for sorting */
    senddate_sort_t * senddate_sort 
    = (senddate_sort_t *)malloc(sizeof(senddate_sort_t) 
    * total_record_number);
    
    senddate_table_entry_t senddate_table_entry;    
    int subdir;
    
    for (i = 0; i < total_record_number; i++) {
        
        if ((i+1) % 1000 == 0) {
            printf("reading %07d of %07d\n", i+1, total_record_number);
        }
        
        /* open the corresponding directory */
        sprintf(filename, "../data/unsorted_senddate");
        if (stat(filename, &st) == -1) {
            fprintf(stderr, "Cannot open %s\n", filename);
            exit(0);
        } 
        subdir = (i / 100000 + 1) * 100000;
        sprintf(filename, "../data/unsorted_senddate/%07d", subdir);
        if (stat(filename, &st) == -1) {
            fprintf(stderr, "Cannot open %s\n", filename);
            exit(0);
        }
        
        /* open the corresponding file */  
        sprintf(filename, 
                "../data/unsorted_senddate/%07d/senddate_%07d.dat", 
                subdir,
                i);
        
        fp = fopen(filename,"rb");
        
        if (!fp) {
            fprintf(stderr, "Cannot open %s\n", filename);
            exit(0);
        }
        
        /* read the data from the file */
        read_senddate(fp, &senddate_table_entry);
        
        senddate_sort[i].year       = senddate_table_entry.year;
        senddate_sort[i].month      = senddate_table_entry.month;
        senddate_sort[i].day        = senddate_table_entry.day;     
        
        strncpy(senddate_sort[i].filename,
                filename,
                TEXT_LONG);     
        
        /* close the file */
        fclose(fp);
    }    
    
    /* sort */    
    printf("sorting...\n");
    qsort(senddate_sort, total_record_number, sizeof(senddate_sort_t), 
          compare_date);
    
    senddate_sort_t curt_senddate;
    int count_unique = 0;
    
    /* remove the duplication */
    printf("removing the duplication\n");
    
    for (i = 0; i < total_record_number; i++) {
        
        if ((i+1) % 1000 == 0) {
            printf("processing %07d of %07d\n", i+1, total_record_number);
        }
        
        if (curt_senddate.year    == senddate_sort[i].year &&
            curt_senddate.month   == senddate_sort[i].month &&
            curt_senddate.day     == senddate_sort[i].day) 
        {
            /* continue if the location is already in location_unique */
            continue;
            
        } else {            
            memcpy(&(curt_senddate), 
                   &(senddate_sort[i]), 
                   sizeof(senddate_sort_t));
            
            fp = fopen(senddate_sort[i].filename,"rb");
            
            if (!fp) {
                fprintf(stderr, "Cannot open %s\n", 
                        senddate_sort[i].filename);
                exit(0);
            }    
            
            /* read the data from the file */
            read_senddate(fp, &senddate_table_entry);
            
            /* close the file */
            fclose(fp);
            
            /* open the corresponding directory */
            sprintf(filename, "../data/unique_senddate");
            if (stat(filename, &st) == -1) {
                mkdir(filename, 0700);
            } 
            subdir = (count_unique / 100000 + 1) * 100000;
            sprintf(filename, "../data/unique_senddate/%07d", subdir);
            if (stat(filename, &st) == -1) {
                mkdir(filename, 0700);
            }
            
            /* open the file */  
            sprintf(filename, 
                    "../data/unique_senddate/%07d/senddate_%07d.dat", 
                    subdir,
                    count_unique);
            
            
            fp = fopen(filename,"wb");
            
            if (!fp) {
                fprintf(stderr, "Cannot open %s\n", filename);
                exit(0);
            }  

			senddate_table_entry.senddate_id = count_unique; 
            
            /* write the unique entry to the file */
            write_senddate(fp, &senddate_table_entry);
            
            /* close the file */
            fclose(fp);
            
            count_unique++;
            
        }
    }    
    
    printf("Total generate %d unique files for the senddate table\n", 
           count_unique);
    
    /* free memory */
    free(senddate_sort);
    
    /* end time */
    gettimeofday(&time_end, NULL);
    
    float totaltime = (time_end.tv_sec - time_start.tv_sec)
    + (time_end.tv_usec - time_start.tv_usec) / 1000000.0f;
    
    
    
    printf("\n\nProcess time %f seconds\n", totaltime);
    
    return 0;
}


