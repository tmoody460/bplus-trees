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
	if (argc < 5){
		printf("Usage: num_users num_locations num_messages num_sendtimes\n");
		exit(0);
	}

    int num_users = atoi(argv[1]);
	int num_locations = atoi(argv[2]);
	int num_messages = atoi(argv[3]);
	int num_times = atoi(argv[4]);  
    struct timeval time_start, time_end;
    gettimeofday(&time_start, NULL);

    int i;
    int user_count = 0;

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

	//binary search timeids
	int times_start = -1, times_end = -1;

	first = 0, last = num_times-1, middle = (first + last)/2;
	sendtime_table_entry_t* time_read = (sendtime_table_entry_t *)malloc(sizeof(sendtime_table_entry_t));
	sendtime_table_entry_t* time_previous = (sendtime_table_entry_t *)malloc(sizeof(sendtime_table_entry_t));
	while (first <= last){
		read_sendtime_better(middle, time_read);
		if (time_read->hour == 8 || (time_read->hour == 9 && time_read->minute == 0)){
			if (time_read->sendtime_id > times_end){
				times_end = time_read->sendtime_id;
			}
			read_sendtime_better(middle-1, time_previous);
			if (time_previous->hour != 8){
				times_start = middle;
				first = last + 1;
			} else {
				last = middle - 1;
			}
		}else if (time_read->hour < 8){
			first = middle + 1;
		}else {
			last = middle - 1;
		}
		middle = (first + last)/2;
	}
	if (times_start == -1){
		printf("\nCouldn't find any times!\n");
		exit(0);
	}
	free(time_previous);
	
	read_sendtime_better(times_end+1, time_read);
	while (time_read->hour == 8 || (time_read->hour == 9 && time_read->minute == 0)){
		times_end++;
		read_sendtime_better(times_end+1, time_read);
	}
	
	free(time_read);	

	//get the first user from nebraska
    user_table_entry_t *user_read = (user_table_entry_t *)malloc(sizeof(user_table_entry_t));
	user_table_entry_t *user_previous = (user_table_entry_t *)malloc(sizeof(user_table_entry_t));
	int first_nebraska_user = -1;   
	first = 0;
	last = num_users-1;
	middle = (first + last)/2;
	while (first <= last){
		read_user_better(middle, user_read);
		if (user_read->location_id == nebraska_start){
			read_user_better(middle-1, user_previous);
			if (user_previous->location_id != nebraska_start){
				//store the filenumber (not user_id)
				first_nebraska_user = middle;
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

	free(user_previous);

	//start a linked list of users
	user_query_entry* user_current = (user_query_entry *)malloc(sizeof(user_query_entry));
	user_current->user.user_id = user_read->user_id;
	user_current->next = NULL;
	
	user_query_entry* user_head = user_current;
	user_query_entry* user_tail = user_current;
	

	int read_next= first_nebraska_user+1;
	read_user_better(read_next, user_read);
	while (user_read->location_id <= nebraska_end){
		//add it to the list
		user_current = (user_query_entry *)malloc(sizeof(user_query_entry));
		user_current->user.user_id = user_read->user_id;
		user_tail->next = user_current;
		user_tail = user_current;

		read_next++;
		read_user_better(read_next, user_read);
	}

	free(user_read);

	//for each user in the list of users, binary search their user id and find their messages
	//to see if any are between 8 and 9	
	message_table_entry_t *message_read = (message_table_entry_t *)malloc(sizeof(message_table_entry_t));
	message_table_entry_t *message_previous = (message_table_entry_t *)malloc(sizeof(message_table_entry_t));

	int user_first_message;

	user_current = user_head;
	while (user_current != NULL){
		first = 0, last = num_messages-1, middle = (first+last)/2;
		while (first <= last){
			read_message_better(middle, message_read);
			if (message_read->user_id == user_current->user.user_id){
				read_message_better(middle-1, message_previous);
				if (message_previous->user_id != user_current->user.user_id){
					user_first_message = middle;
					first = last + 1;
				}else{
					last = middle-1;
				}
			}else if (message_read->user_id < user_current->user.user_id){
				first = middle+1;
			}else{
				last = middle-1;
			}
			middle = (first + last)/2;
		}
		//for this user, go through their messages to see if they have any between 8 and 9
		int done = 0;
		i = 1;
		while (message_read->user_id == user_current->user.user_id && done == 0){
			if (message_read->sendtime_id >= times_start && message_read->sendtime_id <= times_end){
				user_count++;
				done = 1;
			}
			read_message_better(user_first_message + i, message_read);
			i++;
		}
		user_current = user_current->next;
	}
	
	free(message_previous);
	free(message_read);
	free_users(user_head);
	
	//query logic end
	/* end time */
    gettimeofday(&time_end, NULL);
    
    float totaltime = (time_end.tv_sec - time_start.tv_sec)
                    + (time_end.tv_usec - time_start.tv_usec) / 1000000.0f;
                    
    printf("\nThere are %d Nebraska users who sent messages between 8 and 9.", user_count);
    printf("\n\nProcess time %f seconds\n", totaltime);
    
    return 0;
}
