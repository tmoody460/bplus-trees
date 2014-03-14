#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>
#include <string.h>
#include <sys/time.h>

#include "tables.h"
#include "query_helper.h"


//arguments should be in this order:
//number of users
//number of messages
//number of locations

int main(int argc, char **argv)
{
	if (argc < 3){
		printf("Usage: num_users num_locations\n");
		exit(0);
	}

    int num_users = atoi(argv[1]);
	int num_locations = atoi(argv[2]);  
    struct timeval time_start, time_end;
    gettimeofday(&time_start, NULL);

    int i;
    int user_count = 0;

	user_table_entry_t* user_read;
	user_table_entry_t* user_previous;

	location_table_entry_t* location_read;

    //start writing here
    char* nebraska = "Nebraska";
	int nebraska_start = -1;
	int nebraska_end = -1;
    
    //binary search for locationIds from Nebraska
    //requires that locations are sorted by state
	char filename[TEXT_SHORT];
	FILE *fp = NULL;
    int first = 0, last = num_locations-1, middle = (first + last)/2;
	location_read = (location_table_entry_t *)malloc(sizeof(location_table_entry_t));
	location_table_entry_t* location_previous = (location_table_entry_t *)malloc(sizeof(location_table_entry_t));
    while (first <= last){
		read_location_better(middle, location_read);
		//if the middle is nebraska, linearly go back until we find
		//the beginning of the nebraska locations
		if (strcmp(location_read->state, nebraska) == 0){
			if (nebraska_end < middle){
				nebraska_end = middle;
			}
			read_location_better(middle-1, location_previous);
		    //if the previous entry is also nebraska, we're not done
			if (strcmp(location_previous->state, nebraska) < 0){
				nebraska_start = middle;
				first = last + 1;	//this will break out of the while
			} else {
				last = middle - 1;
			}
		}else if (strcmp(location_read->state, nebraska) < 0){
			first = middle + 1;
		}else{
			last = middle - 1;
		}
		middle = (first + last)/2;
    }
	if (nebraska_start  == -1){
		printf("\nCouldn't find any Nebraska users!\n");
		exit(0);
	}

	free(location_previous);

	//get the last nebraksa location_id
	read_location_better(nebraska_end+1, location_read);
	while (strcmp(location_read->state, nebraska) == 0){
		nebraska_end = location_read->location_id;
		read_location_better(nebraska_end+1, location_read);
	}
	free(location_read);
	
	//find the users from nebraska
    user_read = (user_table_entry_t *)malloc(sizeof(user_table_entry_t));
	user_previous = (user_table_entry_t *)malloc(sizeof(user_table_entry_t));
	int first_nebraska_user = -1;   
	first = 0;
	last = num_users-1;
	middle = (first + last)/2;
	while (first <= last){
		read_user_better(middle, user_read);
		if (user_read->location_id == nebraska_start){
			read_user_better(middle-1, user_previous);
			if (user_previous->location_id != nebraska_start){
				user_count++;
				first_nebraska_user = user_read->user_id;
				first = last + 1;	//break out of the loop
			}else{
				last = middle - 1;
			}
		}else if (user_read->location_id < nebraska_start){
			first = middle + 1;
		}else{
			last = middle - 1;
		}
		middle = (first + last)/2;
	}
	if (first_nebraska_user == -1){
		printf("No users from Nebraska found!\n");
		exit(0);
	}
	read_user_better(first_nebraska_user+1, user_read);
	while (user_read->location_id <= nebraska_end){
		user_count++;
		read_user_better(first_nebraska_user+user_count, user_read);
	}

	free(user_read);
	        
    //stop writing here    
    /* end time */
    gettimeofday(&time_end, NULL);
    
    float totaltime = (time_end.tv_sec - time_start.tv_sec)
                    + (time_end.tv_usec - time_start.tv_usec) / 1000000.0f;
                    
    printf("\nThere are %d users from Nebraska.", user_count);
    printf("\n\nProcess time %f seconds\n", totaltime);
    
    return 0;
}
