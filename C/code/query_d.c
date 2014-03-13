#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>
#include <string.h>

#include "record.h"
#include "structures.h"


//arguments should be in this order:
//number of users
//number of messages
//number of locations
//number of date_times

int main(int argc, char **argv)
{
    int num_users = atoi(argv[1]);
	int num_messages = atoi(argv[2]);
	int num_locations = atoi(argv[3]);
	int num_dates = atoi(argv[4]);
	int num_times = atoi(argv[5]);
    struct timeval time_start, time_end;
	
    int i;

    int nebraska_found = 0;
    int nebraska_done = 0;

    int time_found = 0;
    int user_count = 0;

    time* time_head = NULL;
    time* time_tail = NULL;
    time* time_current = NULL;

    location* location_head = NULL;
    location* location_tail;
    location* location_current;

    user* nebraskaUser_head = NULL;
    user* nebraskaUser_tail;
    user* nebraskaUser_current;

    message* message_head = NULL;
	message* message_tail = NULL;
	message* message_current = NULL;

    
	
	long current_user;
	int current_count;
	long max_user;
	int max_count = 0;
	

    /* start time */
    gettimeofday(&time_start, NULL);

    //start writing here

    time all_times[num_times];

    read_all_times(num_times, all_times);


    //Find all timeIds that are between 8 and 9 inclusive
	for (i = 0; i < num_times; i++){
		if (all_times[i].hour == 8 || (all_times[i].hour == 9 && all_times[i].minute == 0)){
			time_current = (time *)malloc(sizeof(time));
			time_current->time_id = all_times[i].time_id;
			time_current->next = NULL;
			if (time_head == NULL){
				time_head = time_current;
				time_tail = time_current;
			} else {
				time_tail->next = time_current;
				time_tail = time_current;
			}
		}else if(all_times[i].hour == 9){
			i = num_times;
		}
	}

    char* nebraska = "Nebraska";
    
    location all_locations[num_locations];

    read_all_locations(num_locations, all_locations);

    //Find all locationIDs that belong to Nebraska
    for (i = 0; i < num_locations; i++) {
        
        if (strstr(all_locations[i].state, nebraska) == NULL && nebraska_found ==1){
           break;
        }

        if (strstr(all_locations[i].state, nebraska) != NULL && nebraska_done == 0){
            nebraska_found = 1;

            location_current = (location *)malloc(sizeof(location));

            location_current->location_id = all_locations[i].location_id;

            location_current->next = NULL;
            
            if (location_head == NULL) {
              location_head = location_current;
              location_tail = location_current;
            } else {
              location_tail->next = location_current;
              location_tail = location_current;
            }
          
        }
    }

    //Find  all usersIds from Nebraska
    user all_users[num_users];

    read_all_users(num_users, all_users);

    
    for (i = 0; i < num_users; i++){

	   location_current = location_head;

    	while (location_current != NULL){

    	    if (all_users[i].location_id == location_current->location_id){
    		        
                nebraskaUser_current = (user *)malloc(sizeof(user));
                
                nebraskaUser_current->user_id = all_users[i].user_id;

                nebraskaUser_current->next = NULL;
                

                if(nebraskaUser_head == NULL){
                    nebraskaUser_head = nebraskaUser_current;
                    nebraskaUser_tail = nebraskaUser_current;
                }else{
                    nebraskaUser_tail->next = nebraskaUser_current;
                    nebraskaUser_tail = nebraskaUser_current;  
                }
    	    }

    	    location_current = location_current->next;
    	}
    }

    //Get all messages
    message all_messages[num_messages];

    read_all_messages(num_messages, all_messages);

    //Find all messages between 8 and 9 inclusive and from Nebraska
    for (i = 0; i < num_messages; i++){
    	     
        time_current = time_head;
        //Match correct time_ids first
        while(time_current != NULL){
        
            if(all_messages[i].time_id == time_current->time_id){

                nebraskaUser_current = nebraskaUser_head;              
                //Match correct user_id
                while(nebraskaUser_current != NULL){

                    if(all_messages[i].user_id == nebraskaUser_current->user_id){
		
						message_current = (message *)malloc(sizeof(message));
						message_current->user_id = all_messages[i].user_id;
						message_current->next = NULL;
						
						if(message_head == NULL){
							message_head = message_current;
							message_tail = message_current;
						}else{
							message_tail->next = message_current;
							message_tail = message_current;
						}

                    }
                   
                    nebraskaUser_current = nebraskaUser_current->next;
  
                }
            }
 
            time_current = time_current->next;
 
        }      
    }
	
	message_current = message_head;
	current_user = message_current->user_id;
	current_count = 0;
	while (message_current != NULL){
		if (message_current->user_id == current_user){
			current_count++;
		}else{
			if(current_count > max_count){
				max_count = current_count;
				max_user = current_user;
			}
			current_user = message_current->user_id;
			current_count = 1;
		}
		message_current = message_current->next;
	}
	
    //stop writing here    
    
    /* end time */
    gettimeofday(&time_end, NULL);
    
    float totaltime = (time_end.tv_sec - time_start.tv_sec)
                    + (time_end.tv_usec - time_start.tv_usec) / 1000000.0f;
                    
	//free all the linked lists
	free_messages(message_head);
	free_locations(location_head);
	free_times(time_head);
	free_users(nebraskaUser_head);
	         
    printf("\nUser %li from Nebraska had %d messages between 8 and 9.", max_user, max_count);
    printf("\n\nProcess time %f seconds\n", totaltime);
    
    return 0;
}
