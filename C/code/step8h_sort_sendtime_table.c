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
    int hour;
	int minute; 
    char filename[TEXT_LONG];
} sendtime_sort_t;

/*
 * the auxiliary compare function
 * 
 */
int compare_time(const void *a, const void *b) 
{
	sendtime_sort_t *ta = (sendtime_sort_t *)(a);
	sendtime_sort_t *tb = (sendtime_sort_t *)(b);
	if (ta->hour != tb->hour){
		return (ta->hour < tb->hour);
	}

	return (ta->minute < tb->minute);
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
    sendtime_sort_t * sendtime_sort 
        = (sendtime_sort_t *)malloc(sizeof(sendtime_sort_t) 
                                        * total_record_number);
        
    sendtime_table_entry_t sendtime_table_entry;    
    int subdir;
    
    for (i = 0; i < total_record_number; i++) {
        
        if ((i+1) % 1000 == 0) {
            printf("reading %07d of %07d\n", i+1, total_record_number);
        }
        
        /* open the corresponding directory */
        sprintf(filename, "../data/unique_sendtime");
        if (stat(filename, &st) == -1) {
            fprintf(stderr, "Cannot open %s\n", filename);
            exit(0);
        } 
        subdir = (i / 100000 + 1) * 100000;
        sprintf(filename, "../data/unique_sendtime/%07d", subdir);
        if (stat(filename, &st) == -1) {
            fprintf(stderr, "Cannot open %s\n", filename);
            exit(0);
        }
        
        /* open the corresponding file */  
        sprintf(filename, 
                "../data/unique_sendtime/%07d/sendtime_%07d.dat", 
                subdir,
                i);
    
        fp = fopen(filename,"rb");
    
        if (!fp) {
            fprintf(stderr, "Cannot open %s\n", filename);
            exit(0);
        }
        
        /* read the data from the file */
        read_sendtime(fp, &sendtime_table_entry);

        sendtime_sort[i].hour       = sendtime_table_entry.hour;
        sendtime_sort[i].minute       = sendtime_table_entry.minute;
        strncpy(sendtime_sort[i].filename,
                filename,
                TEXT_LONG);     

        /* close the file */
        fclose(fp);
    }    
    
    /* sort */    
    printf("sorting...\n");
    qsort(sendtime_sort, total_record_number, sizeof(sendtime_sort_t), 
          compare_time);
    
    /* generate the files according to the sorted key */
    for (i = 0; i < total_record_number; i++) {
        
        if ((i+1) % 1000 == 0) {
            printf("processing %07d of %07d\n", i+1, total_record_number);
        }
        
        fp = fopen(sendtime_sort[i].filename,"rb");
        
        if (!fp) {
            fprintf(stderr, "Cannot open %s\n", 
                    sendtime_sort[i].filename);
            exit(0);
        }    
        
        /* read the data from the file */
        read_sendtime(fp, &sendtime_table_entry);
        
        /* close the file */
        fclose(fp);
        
        /* open the corresponding directory */
        sprintf(filename, "../data/sorted_sendtime");
        if (stat(filename, &st) == -1) {
            mkdir(filename, 0700);
        } 
        subdir = (i / 100000 + 1) * 100000;
        sprintf(filename, "../data/sorted_sendtime/%07d", subdir);
        if (stat(filename, &st) == -1) {
            mkdir(filename, 0700);
        }
        
        /* open the file */  
        sprintf(filename, 
                "../data/sorted_sendtime/%07d/sendtime_%07d.dat", 
                subdir,
                i);
        
        
        fp = fopen(filename,"wb");
        
        if (!fp) {
            fprintf(stderr, "Cannot open %s\n", filename);
            exit(0);
        }   
        
        /* write the unique entry to the file */
        write_sendtime(fp, &sendtime_table_entry);
        
        /* close the file */
        fclose(fp);
    }
        
    printf("Total generate %d files for the sorted sendtime table\n", 
           total_record_number);
    
    /* free memory */
    free(sendtime_sort);
    
    /* end time */
    gettimeofday(&time_end, NULL);
    
    float totaltime = (time_end.tv_sec - time_start.tv_sec)
                    + (time_end.tv_usec - time_start.tv_usec) / 1000000.0f;
                    
         
                    
    printf("\n\nProcess time %f seconds\n", totaltime);
    
    return 0;
}


