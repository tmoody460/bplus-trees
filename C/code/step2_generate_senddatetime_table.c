#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#include "tables.h"

int main(int argc, char **argv)
{
    int i, j;
    struct stat st;
        
    /* print usage if needed */
    if (argc != 2) {
        fprintf(stderr, "Usage: %s total_record_number\n", argv[0]);
        exit(0);
    }
    
    /* get total record number */
    int total_record_number = atoi(argv[1]);
    
    char filename[1024];
    FILE *fp = NULL;
	
    
    struct timeval time_start, time_end;
    
    /* start time */
    gettimeofday(&time_start, NULL);
    
    senddate_table_entry_t senddate_table_entry; 
    sendtime_table_entry_t sendtime_table_entry;       
    
	int count = 0;
    int subdir = 0;
    for (i = 0; i < total_record_number; i++) {
        
        if ((i+1) % 1000 == 0) {
            printf("processing %06d of %06d\n", i+1, total_record_number);
        }
        
        /* open the corresponding file */  
        sprintf(filename, "../data/original/record_%06d.dat", i);
    
        fp = fopen(filename,"rb");
    
        if (!fp) {
            fprintf(stderr, "Cannot open %s\n", filename);
            exit(0);
        }
        
        /* read the record from the file */
        record_t *rp = read_record(fp);
        
        /* close the file */
        fclose(fp);
    
        /* =========== start of data processing code ================ */
        
        for (j = 0; j < rp->message_num; j++) { 
            
            /* copy the senddatetime table entry */
            senddate_table_entry.senddate_id    = count;
			sendtime_table_entry.sendtime_id	= count;
            senddate_table_entry.year   = rp->messages[j].year;
            senddate_table_entry.month  = rp->messages[j].month;
            senddate_table_entry.day    = rp->messages[j].day;
            sendtime_table_entry.hour   = rp->messages[j].hour;
            sendtime_table_entry.minute = rp->messages[j].minute;

			
            
			//DATE     
            /* open the corresponding directory */
            sprintf(filename, "../data/unsorted_senddate");
            if (stat(filename, &st) == -1) {
                mkdir(filename, 0700);
            } 
            subdir = (count / 100000 + 1) * 100000;
            sprintf(filename, "../data/unsorted_senddate/%07d", subdir);
            if (stat(filename, &st) == -1) {
                mkdir(filename, 0700);
            }
        
            /* open the corresponding file */  
            sprintf(filename, 
                    "../data/unsorted_senddate/%07d/senddate_%07d.dat", 
                    subdir,
                    count);
            fp = fopen(filename,"wb");
        
            if (!fp) {
                fprintf(stderr, "Cannot open %s\n", filename);
                exit(0);
            }
        
            /* write the senddatetime to the file */
            write_senddate(fp, &senddate_table_entry);
        
            /* close the file */
            fclose(fp);

			//TIME
            /* open the corresponding directory */
            sprintf(filename, "../data/unsorted_sendtime");
            if (stat(filename, &st) == -1) {
                mkdir(filename, 0700);
            } 
            subdir = (count / 100000 + 1) * 100000;
            sprintf(filename, "../data/unsorted_sendtime/%07d", subdir);
            if (stat(filename, &st) == -1) {
                mkdir(filename, 0700);
            }
        
            /* open the corresponding file */  
            sprintf(filename, 
                    "../data/unsorted_sendtime/%07d/sendtime_%07d.dat", 
                    subdir,
                    count);
            fp = fopen(filename,"wb");
        
            if (!fp) {
                fprintf(stderr, "Cannot open %s\n", filename);
                exit(0);
            }
        
            /* write the senddatetime to the file */
            write_sendtime(fp, &sendtime_table_entry);
        
            /* close the file */
            fclose(fp);

			count ++;
        }
        
        /* =========== end of data processing code ================ */    
    
        /* free memory */
        free_record(rp);
    }            
    
    printf("Total generate %d files for the senddate and sendtime tables\n", count);
    
    /* end time */
    gettimeofday(&time_end, NULL);
    
    float totaltime = (time_end.tv_sec - time_start.tv_sec)
                    + (time_end.tv_usec - time_start.tv_usec) / 1000000.0f;
         
                    
    printf("\n\nProcess time %f seconds\n", totaltime);
    
    return 0;
}


