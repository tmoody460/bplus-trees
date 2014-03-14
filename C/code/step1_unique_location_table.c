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
    char city[TEXT_SHORT];
    char state[TEXT_SHORT];
    char filename[TEXT_LONG];
} location_sort_t;

/*
 * the auxiliary compare function
 * 
 */
int compare(const void *a, const void *b) 
{
		if (strcmp(((location_sort_t *)(a))->state, ((location_sort_t *)(b))->state) != 0){
        	return strcmp(((location_sort_t *)(a))->state, ((location_sort_t *)(b))->state);
		}else{
        	return strcmp(((location_sort_t *)(a))->city, ((location_sort_t *)(b))->city);
		}

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
    location_sort_t * location_sort 
        = (location_sort_t *)malloc(sizeof(location_sort_t) 
                                    * total_record_number);
        
    location_table_entry_t location_table_entry;    
    int subdir;
    
    for (i = 0; i < total_record_number; i++) {
        
        if ((i+1) % 1000 == 0) {
            printf("reading %07d of %07d\n", i+1, total_record_number);
        }
                
        /* open the corresponding directory */
        sprintf(filename, "../data/unsorted_location");
        if (stat(filename, &st) == -1) {
            fprintf(stderr, "Cannot open %s\n", filename);
            exit(0);
        } 
        subdir = (i / 100000 + 1) * 100000;
        sprintf(filename, "../data/unsorted_location/%07d", subdir);
        if (stat(filename, &st) == -1) {
            fprintf(stderr, "Cannot open %s\n", filename);
            exit(0);
        }
        
        /* open the corresponding file */
        sprintf(filename, "../data/unsorted_location/%07d/location_%07d.dat", 
                subdir,
                i);
    
        fp = fopen(filename,"rb");
    
        if (!fp) {
            fprintf(stderr, "Cannot open %s\n", filename);
            exit(0);
        }
        
        /* read the data from the file */
        read_location(fp, &location_table_entry);
    
		strncpy(location_sort[i].city, 
        		location_table_entry.city,
        		TEXT_SHORT);

        strncpy(location_sort[i].state, 
                location_table_entry.state,
                TEXT_SHORT);
        
        strncpy(location_sort[i].filename, 
                filename,
                TEXT_LONG); 
            
        /* close the file */
        fclose(fp);
    }   
        
    /* sort */    
    printf("sorting...\n");
    qsort(location_sort, total_record_number, sizeof(location_sort_t), compare);
    
    char curt_state[TEXT_SHORT];
	char curt_city[TEXT_SHORT];
    int count_unique = 0;
    
    /* remove the duplication */
    printf("removing the duplication\n");
    for (i = 0; i < total_record_number; i++) {
                
        if ((i+1) % 1000 == 0) {
            printf("processing %07d of %07d\n", i+1, total_record_number);
        }
        
        if (strcmp(location_sort[i].city, 
                   curt_city) == 0 && strcmp(location_sort[i].state, curt_state) == 0) 
        {
            /* continue if we already process it */
            continue;
            
        } else {            
            strncpy(curt_state,
                    location_sort[i].state,
                    TEXT_SHORT);

            strncpy(curt_city,
                    location_sort[i].city,
                    TEXT_SHORT);
                   
            fp = fopen(location_sort[i].filename,"rb");
    
            if (!fp) {
                fprintf(stderr, "Cannot open %s\n", 
                        location_sort[i].filename);
                exit(0);
            }    
        
            /* read the location from the file */
            read_location(fp, &location_table_entry);
        
            /* close the file */
            fclose(fp);
        
            /* open the corresponding directory */
            sprintf(filename, "../data/unique_location");
            if (stat(filename, &st) == -1) {
                mkdir(filename, 0700);
            } 
            subdir = (count_unique / 100000 + 1) * 100000;
            sprintf(filename, "../data/unique_location/%07d", subdir);
            if (stat(filename, &st) == -1) {
                mkdir(filename, 0700);
            }
        
            /* open the file */  
            sprintf(filename, 
                    "../data/unique_location/%07d/location_%07d.dat", 
                    subdir,
                    count_unique);
    
            fp = fopen(filename,"wb");
    
            if (!fp) {
                fprintf(stderr, "Cannot open %s\n", filename);
                exit(0);
            }

			location_table_entry.location_id = count_unique;

            /* write the unique entry to the file */
            write_location(fp, &location_table_entry);
                        
            /* close the file */
            fclose(fp);
            
            /* increase the number of unique entries */
            count_unique++;
        }
    }
    
    printf("Total generate %d unique files for the sorted location table\n", count_unique);
    
    /* free memory */
    free(location_sort);    
    
    /* end time */
    gettimeofday(&time_end, NULL);
    
    float totaltime = (time_end.tv_sec - time_start.tv_sec)
                    + (time_end.tv_usec - time_start.tv_usec) / 1000000.0f;
                             
                    
    printf("\n\nProcess time %f seconds\n", totaltime);
    
    return 0;
}


