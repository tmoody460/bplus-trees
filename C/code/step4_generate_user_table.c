#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#include "tables.h"

/*
 * the auxiliary data struct for lookup
 * 
 */
typedef struct {                                                   
    int  location_id;               
    char city[TEXT_SHORT];
	char state[TEXT_SHORT];
} location_lookup_t;


/*
 * search the location id according to the location name  using binary searh
 * 
 */
int look_up_location_id(char location[TEXT_SHORT], 
                        int total_location_number,
                        location_lookup_t *location_lookup)
{
    int imin = 0;
    int imax = total_location_number - 1;

	char *buf;
	char **bp;
	char* city;
	char* state;
	
	if(location != NULL && strcmp(location, "") != 0 && strcmp(location, " ") != 0) {
		buf = location;
		bp = &buf;
		//split city and state		
		city = strsep(bp, ",");
    	state = strsep(bp, ",");
	} else {
		city = "";
		state = "";
	}
    
    while (imax >= imin) {
        int imid = (imax + imin) / 2;
        int res, res2;
                
        res=strcmp(location_lookup[imid].state, state);
        
        if (res == 0) {
			res2=strcmp(location_lookup[imid].city, city);
			if (res2 == 0) {
            	return location_lookup[imid].location_id;
			} else if (res2 < 0){
				imin = imid + 1;
			} else {
				imax = imid;
			}
        } else if (res < 0) {
            imin = imid + 1;
        } else {
            imax = imid;
        }
    }
    
    fprintf(stderr, "Cannot find the location %s, %s\n", city, state);
    return -1;
}

int main(int argc, char **argv)
{
    int i, j;
    struct stat st;
        
    /* print usage if needed */
    if (argc != 3) {
        fprintf(stderr, "Usage: %s total_record_number total_location_number\n", argv[0]);
        exit(0);
    }
    
    /* get total record number */
    int total_record_number = atoi(argv[1]);
    
    /* get total location number */
    int total_location_number = atoi(argv[2]);
    
    char filename[1024];
    FILE *fp = NULL;
    
    struct timeval time_start, time_end;
    
    /* start time */
    gettimeofday(&time_start, NULL);
    
    /* prepare the location lookup table */
    location_lookup_t *location_lookup 
        = (location_lookup_t *)malloc(sizeof(location_lookup_t) 
                                      * total_location_number);
    int subdir = 0;    
    location_table_entry_t location_table_entry;    
    for (i = 0; i < total_location_number; i++) {
		if (i % 1000 == 0)
        	printf("Reading Location #%d of %d\n", i, total_location_number);
        /* open the corresponding directory */
        sprintf(filename, "../data/unique_location");
        if (stat(filename, &st) == -1) {
            fprintf(stderr, "Cannot open %s\n", filename);
            exit(0);
        } 
        subdir = (i / 100000 + 1) * 100000;
        sprintf(filename, "../data/unique_location/%07d", subdir);
        if (stat(filename, &st) == -1) {
            fprintf(stderr, "Cannot open %s\n", filename);
            exit(0);
        }
        
        /* open the corresponding file */  
        sprintf(filename, 
                "../data/unique_location/%07d/location_%07d.dat", 
                subdir,
                i);
           
        fp = fopen(filename,"rb");
    
        if (!fp) {
            fprintf(stderr, "Cannot open %s\n", filename);
            exit(0);
        } 
        
        /* read the data from the file */
        read_location(fp, &location_table_entry);
        fclose(fp);  
        
        location_lookup[i].location_id = location_table_entry.location_id;
        strncpy(location_lookup[i].state, 
                location_table_entry.state, 
                TEXT_SHORT);
        strncpy(location_lookup[i].city, 
                location_table_entry.city, 
                TEXT_SHORT);
    }
    
    user_table_entry_t user_table_entry;    
    int count = 0;

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
        
        /* copy the senddatetime table entry */
        user_table_entry.user_id = rp->id;
        strncpy(user_table_entry.user_name, rp->name, TEXT_SHORT);
        user_table_entry.location_id = look_up_location_id(rp->location, 
                                                           total_location_number,
                                                           location_lookup);
        
        /* open the corresponding directory */
        sprintf(filename, "../data/unsorted_user");
        if (stat(filename, &st) == -1) {
            mkdir(filename, 0700);
        } 
        subdir = (count / 100000 + 1) * 100000;
        sprintf(filename, "../data/unsorted_user/%07d", subdir);
        if (stat(filename, &st) == -1) {
            mkdir(filename, 0700);
        }
        
        /* open the corresponding file */  
        sprintf(filename, 
                "../data/unsorted_user/%07d/user_%07d.dat", 
                subdir,
                count);
        fp = fopen(filename,"wb");
        
        if (!fp) {
            fprintf(stderr, "Cannot open %s\n", filename);
            exit(0);
        }
        
        count++;
        
        /* write the senddatetime to the file */
        write_user(fp, &user_table_entry);
        
        /* close the file */
        fclose(fp);
        
        
        /* =========== end of data processing code ================ */    
        
        /* free memory */
        free_record(rp);
    }          
    
    free(location_lookup);
    
    printf("Total generate %d files for the user table\n", count);
    
    /* end time */
    gettimeofday(&time_end, NULL);
    
    float totaltime = (time_end.tv_sec - time_start.tv_sec)
                    + (time_end.tv_usec - time_start.tv_usec) / 1000000.0f;
         
                    
    printf("\n\nProcess time %f seconds\n", totaltime);
    
    return 0;
}


