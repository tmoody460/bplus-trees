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
    int  user_id; 
    char filename[TEXT_LONG];
} user_sort_t;

/*
 * the auxiliary compare function
 * 
 */
int compare_userid(const void *a, const void *b) 
{
    return (((user_sort_t *)(a))->user_id) >
           (((user_sort_t *)(b))->user_id);
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
    user_sort_t * user_sort 
        = (user_sort_t *)malloc(sizeof(user_sort_t) 
                                * total_record_number);
        
    user_table_entry_t user_table_entry;    
    int subdir;
    
    for (i = 0; i < total_record_number; i++) {
        
        if ((i+1) % 1000 == 0) {
            printf("reading %07d of %07d\n", i+1, total_record_number);
        }
        
        /* open the corresponding directory */
        sprintf(filename, "../data/unsorted_user");
        if (stat(filename, &st) == -1) {
            fprintf(stderr, "Cannot open %s\n", filename);
            exit(0);
        } 
        subdir = (i / 100000 + 1) * 100000;
        sprintf(filename, "../data/unsorted_user/%07d", subdir);
        if (stat(filename, &st) == -1) {
            fprintf(stderr, "Cannot open %s\n", filename);
            exit(0);
        }
        
        /* open the corresponding file */  
        sprintf(filename, 
                "../data/unsorted_user/%07d/user_%07d.dat", 
                subdir,
                i);
    
        fp = fopen(filename,"rb");
    
        if (!fp) {
            fprintf(stderr, "Cannot open %s\n", filename);
            exit(0);
        }
        
        /* read the data from the file */
        read_user(fp, &user_table_entry);
        
        user_sort[i].user_id            = user_table_entry.user_id;        
        strncpy(user_sort[i].filename,
                filename,
                TEXT_LONG);     

        /* close the file */
        fclose(fp);
    }    
    
    /* sort */    
    printf("sorting...\n");
    qsort(user_sort, total_record_number, sizeof(user_sort_t), 
          compare_userid);

    int curt_user_id;
    int count_unique = 0;
    
    /* remove the duplication according to user id*/
    printf("removing the duplication\n");
    
    for (i = 0; i < total_record_number; i++) {
        
        if ((i+1) % 1000 == 0) {
            printf("processing %07d of %07d\n", i+1, total_record_number);
        }
        
        if (curt_user_id == user_sort[i].user_id) 
        {
            continue;
            
        } else {            
            curt_user_id = user_sort[i].user_id;
            
            fp = fopen(user_sort[i].filename,"rb");
            
            if (!fp) {
                fprintf(stderr, "Cannot open %s\n", 
                        user_sort[i].filename);
                exit(0);
            }    
            
            /* read the data from the file */
            read_user(fp, &user_table_entry);
            
            /* close the file */
            fclose(fp);
            
            /* open the corresponding directory */
            sprintf(filename, "../data/unique_user");
            if (stat(filename, &st) == -1) {
                mkdir(filename, 0700);
            } 
            subdir = (count_unique / 100000 + 1) * 100000;
            sprintf(filename, "../data/unique_user/%07d", subdir);
            if (stat(filename, &st) == -1) {
                mkdir(filename, 0700);
            }
            
            /* open the file */  
            sprintf(filename, 
                    "../data/unique_user/%07d/user_%07d.dat", 
                    subdir,
                    count_unique);
            
            
            fp = fopen(filename,"wb");
            
            if (!fp) {
                fprintf(stderr, "Cannot open %s\n", filename);
                exit(0);
            }   
            
			//user_table_entry.user_id = count_unique;

            /* write the unique entry to the file */
            write_user(fp, &user_table_entry);
            
            /* close the file */
            fclose(fp);
            
            count_unique++;
        }
    }
    
    printf("Total generate %d files for the sorted user table\n", 
           count_unique);
    
    /* free memory */
    free(user_sort);
    
    /* end time */
    gettimeofday(&time_end, NULL);
    
    float totaltime = (time_end.tv_sec - time_start.tv_sec)
                    + (time_end.tv_usec - time_start.tv_usec) / 1000000.0f;
                    
         
                    
    printf("\n\nProcess time %f seconds\n", totaltime);
    
    return 0;
}


