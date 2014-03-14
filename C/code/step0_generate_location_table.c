#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#include "tables.h"

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
    
    char filename[1024];
    FILE *fp = NULL;
    
    struct timeval time_start, time_end;
    
    /* start time */
    gettimeofday(&time_start, NULL);
    
    location_table_entry_t location_table_entry;    
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
        
        /* copy the table entry */
        location_table_entry.location_id = count;
        //strncpy(&(location_table_entry.location[0]), &(rp->location[0]), TEXT_SHORT);
	
	//new code
	char *buf;
	char **bp;
	char* city;
	char* state;
	
	if(rp->location != NULL && strcmp(rp->location, "") != 0 && strcmp(rp->location, " ") != 0) {
		buf = rp->location;
		bp = &buf;
		//split city and state		
		city = strsep(bp, ",");
    	state = strsep(bp, ",");
	} else {
		city = "";
		state = "";
	}
	strcpy(&(location_table_entry.state[0]), state);
	strcpy(&(location_table_entry.city[0]), city);
	//end new code
        
        /* open the corresponding directory */
        sprintf(filename, "../data/unsorted_location");
        if (stat(filename, &st) == -1) {
            mkdir(filename, 0700);
        } 
        subdir = (count / 100000 + 1) * 100000;
        sprintf(filename, "../data/unsorted_location/%07d", subdir);
        if (stat(filename, &st) == -1) {
            mkdir(filename, 0700);
        } 
        
        /* open the corresponding file */
        sprintf(filename, "../data/unsorted_location/%07d/location_%07d.dat", 
                subdir,
                count);
        fp = fopen(filename,"wb");
        
        if (!fp) {
            fprintf(stderr, "Cannot open %s\n", filename);
            exit(0);
        }

        count++;
        
        /* write the location to the file */
        write_location(fp, &location_table_entry);
        
        /* close the file */
        fclose(fp);
        
        /* =========== end of data processing code ================ */    
    
        /* free memory */
        free_record(rp);
    

    }     
    
    printf("Total generate %d files for the location table\n", count);
    
    /* end time */
    gettimeofday(&time_end, NULL);
    
    float totaltime = (time_end.tv_sec - time_start.tv_sec)
                    + (time_end.tv_usec - time_start.tv_usec) / 1000000.0f;
         
                    
    printf("\n\nProcess time %f seconds\n", totaltime);
    
    return 0;
}


