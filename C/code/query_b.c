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
int binary_search(int key, int start, int end, node* current_node)

int main(int argc, char **argv)
{
    int num_users = atoi(argv[1]);
	int num_messages = atoi(argv[2]);
	int num_locations = atoi(argv[3]);
	int num_dates = atoi(argv[4]);
	int num_times = atoi(argv[5]);   
	
    struct timeval time_start, time_end;
    int i;

    int user_count = 0;
    int user;

    int found_end = 0;

    int key_index;

    user_query_entry* user_head=NULL;
	user_query_entry* user_tail;
	user_query_entry* user_current;
	int user_found = 0;

    /* start time */
    gettimeofday(&time_start, NULL);
    //start writing here

    char filename[TEXT_SHORT];

    time* time_struct;
    message* message_struct;

    FILE *fp = NULL;

    /*Beginning logic for finding first timeId */

    //Find first node with correct time key
    node* first_node = find_first_time_node(8.0);

    //Binary search node for time key
    key_index = binary_search(8.0, 0, first_node->num_filled, first_node);

    //Access child that has time key and get timeID
    sprintf(filename, "times/%c", first_node->children[key_index-1]);
    fp = fopen(filename,"rb");
    read_location(fp, time_struct);
    int first_time = time_struct->time_id;
    fpclose(fp);

    /*End of logic for finding first timeID*/

    /*Beginning logic for finding last timeID*/
    first_node = find_first_time_node(9.0);
 	
 	//Binary search node for time key
    key_index = binary_search(9.0, 0, first_node->num_filled, first_node);

    //Access child that has time key and get timeID
    sprintf(filename, "times/%c", current_node->children[key_index-1]);
    fp = fopen(filename,"rb");
    read_location(fp, time_struct);
    int last_time = time_struct->time_id;

    fpclose(fp);

    /*End of logic for finding last timeID */
    
    /*Beginning logic for finding first node with message sent between 8 and 9*/
    int time_key = first_time;
    node* first_node = find_first_time_node(time_key);
    
    while(first_node == NULL && time_key <= last_time){
    	time_key++;
    	first_node = find_first_time_node(time_key);
    }

    /*End logic for finding first node with message sent between 8 and 9*/

	/*Beginning logic for finding number of users who sent message between 8 and 9*/
    if(first_node != NULL){
    	index = binary_search(time_key, 0, first_node->num_filled, first_node);

    	//Binary search will not necessarily give the first message 
	    while((key_index >= 0) && (found_end == 0)){
	        if(first_node->keys[index] == time_key){
	            key_index--;
	        }
	        else{
	            found_end=1
	            key_index++;;
	        }
	    }

	    node* current_node = first_node;
	    int last_user_found = 0;
	    while (current_node != NULL){

	    	while (key_index < current_node->num_filled){
	    		if(curret_node->keys[key_index] > last_time){
	    			last_user_found = 1;
	    			break;
	    		}
	    		else{

	    			//Read message and find userId
	    			sprintf(filename, "messages/%c", current_node->children[key_index]);
				    fp = fopen(filename,"rb");
				    read_message(fp, message_struct);
				    user = message_struct->user_id;

				    fpclose(fp);
				    
				    user_current = (user_query_entry*)malloc(sizeof(user_query_entry));

				    //Check to see if we already counted that user
		    		user_current = user_head;
					while (user_current != NULL && user_found == 0){
						if (user_current->user.user_id == user){
							user_found = 1;
						}
						user_current = user_current->next;
					}
					//If we haven't counted, then count it and add user id to the list of counted users
					if (user_found == 0){
					

						user_current = (user *)malloc(sizeof(user));
						user_current->user.user_id = user;
						user_current->next = NULL;
						if(user_head == NULL){
							user_head = user_current;
							user_tail = user_current;
						}else{
							user_tail->next = user_current;
							user_tail = user_current;
						}
						user_count++;
					}
					user_found = 0;
					key_index++;
				}
	    	}

	    	if(last_user_found == 0){
	    		key_index = 0;
	    		read_node(current_node, current_node->children[(current_node->num_filled)-1]);
	    	}
	    	else{
	    		break;
	    	}
		}
    }


    /*End logic for finding number of users who sent messages between 8 and 9*/

    //stop writing here    
    
    /* end time */
    gettimeofday(&time_end, NULL);
    
    float totaltime = (time_end.tv_sec - time_start.tv_sec)
                    + (time_end.tv_usec - time_start.tv_usec) / 1000000.0f;
                    
    free_locations(location_head);
    printf("\nThere are %d users from Nebraska.", user_count);
    printf("\n\nProcess time %f seconds\n", totaltime);
    
    return 0;
}

int binary_search(int key, int start, int end, node current_node){

    int middle = (start + end)/2;

    if(current_node->keys[middle] == key)
    {
        return middle;
    }
    else {
        if(key > current_node->keys[middle])
            return binarysearch(key, middle, end, current_node);
        else
        if(key < current_node->leys[middle])
            return binarysearch(key, start, middle, current_node);
    }
   //if it is not found in the whole array
   return -1;

}

int last_binary_search(int key, int start, int end, node current_node){

    int middle = (start + end)/2;

    if(current_node->keys[middle] <= key)
    {
        return middle;
    }
    else {
        return binarysearch(key, start, middle, current_node);
    }
   //if it is not found in the whole array
   return -1;

}