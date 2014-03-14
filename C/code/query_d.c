#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/times.h>
#include <string.h>

#include "tables.h"
#include "search.c"
#include "query_helper.h"
#include "bp_structs.c"
char *nebraska = "Nebraska";
int nebraska_binary_search(int start, int end, location_node* current_node){

    int middle = (start + end)/2;

    int comparison = strcmp(current_node->keys[middle], nebraska);

    if(comparison == 0)
    {
        return middle;
    }
    else{
        return nebraska_binary_search(start, middle, current_node);
    }
   
   //if it is not found in the whole array
   return -1;

}

int binary_search(int key, int start, int end, message_node* current_node){

    int middle = (start + end)/2;

    if(current_node->keys[middle] == key)
    {
        return middle;
    }
    else {
        if(key > current_node->keys[middle])
            return binary_search(key, middle, end, current_node);
        else
        if(key < current_node->keys[middle])
            return binary_search(key, start, middle, current_node);
    }
   //if it is not found in the whole array
   return -1;

}

int user_binary_search(int key, int start, int end, user_node* current_node){

    int middle = (start + end)/2;

    if(current_node->keys[middle] == key)
    {
        return middle;
    }
    else {
        if(key > current_node->keys[middle])
            return user_binary_search(key, middle, end, current_node);
        else
        if(key < current_node->keys[middle])
            return user_binary_search(key, start, middle, current_node);
    }
   //if it is not found in the whole array
   return -1;

}

int time_binary_search(int key, int start, int end, time_node* current_node){

    int middle = (start + end)/2;

    if(current_node->keys[middle] == key)
    {
        return middle;
    }
    else {
        if(key > current_node->keys[middle])
            return time_binary_search(key, middle, end, current_node);
        else
        if(key < current_node->keys[middle])
            return time_binary_search(key, start, middle, current_node);
    }
   //if it is not found in the whole array
   return -1;

}

int last_binary_search(int key, int start, int end, message_node* current_node){

    int middle = (start + end)/2;

    if(current_node->keys[middle] <= key)
    {
        return middle;
    }
    else {
        return last_binary_search(key, start, middle, current_node);
    }
   //if it is not found in the whole array
   return -1;

}

int main(int argc, char **argv)
{
    int num_users = atoi(argv[1]);
    int num_messages = atoi(argv[2]);
    int num_locations = atoi(argv[3]);
    int num_dates = atoi(argv[4]);
    int num_times = atoi(argv[5]);   
    
    struct timeval time_start, time_end;
    int i = 0;

    int user_count = 0;

    int node_not_found = 0;

    int found_end = 0;

    int key_index;

    user_query_entry* user_head=NULL;
    user_query_entry* user_tail;
    user_query_entry* user_current;
    int user_found = 0;

    int found_message;

    /* start time */
    gettimeofday(&time_start, NULL);
    //start writing here

    char filename[TEXT_SHORT];

    location_table_entry_t* location_struct;

    FILE *fp = NULL;

    /*Beginning logic for finding first Nebraska locationId */

    //Find first node with Nebraska key
    location_node* first_location_node = (location_node*)malloc(sizeof(location_node)); 
    search_location_tree(nebraska, first_location_node);

    //Binary search node for Nebraska key
    key_index = nebraska_binary_search(0, first_location_node->num_filled-1, first_location_node);

    //Binary search will not necessarily give the first Nebraska key 
    while((key_index >= 0) && (found_end == 0)){
        if(strstr(first_location_node->keys[key_index-1], nebraska) != NULL){
            key_index--;
        }
        else{
            found_end=1;
            key_index++;
        }
    }

    //Access child that has Nebraska key and get locationID
    sprintf(filename, "locations/%s", first_location_node->children[key_index-1]);
    fp = fopen(filename,"rb");
    read_location(fp, location_struct);
    int first_Nebraska = location_struct->location_id;
    fclose(fp);

    /*End of logic for finding first Nebraska locationId*/

    /*Beginning logic for finding last Nebraska locationId */

    location_node* current_node = first_location_node;
    location_node* previous_node = (location_node*)malloc(sizeof(location_node));
    
    found_end = 0;
    key_index = -1;
    while(current_node != NULL && found_end == 0){
        
        int comparison = strcmp(current_node->keys[(current_node->num_filled)-2], nebraska);

        if(comparison == 0){
            previous_node = current_node;
            if(current_node->keys[FAN_OUT-1] != NULL){
                read_location_node(current_node, current_node->keys[FAN_OUT-1]);
            }
            else{
                current_node = NULL;
            }

        }
        else if(comparison > 0){
            key_index = nebraska_binary_search(0, current_node->num_filled-1, current_node);
            
            //Binary search will not necessarily give the last Nebraska key 
            while((key_index < current_node->num_filled) && (found_end == 0)){
                if(strstr(current_node->keys[key_index+1], nebraska) != NULL){
                    key_index++;
                }
                else{
                    found_end=1;
                }
            }
        }        

    }

    if(key_index == -1){
        current_node = previous_node;
        key_index = current_node->num_filled;
    }

    sprintf(filename, "locations/%s", current_node->children[key_index-1]);
    fp = fopen(filename,"rb");
    read_location(fp, location_struct);
    int last_Nebraska = location_struct->location_id;

    fclose(fp);

    /*End of logic for finding last Nebraska locationId */
    
    
    /*Beginning logic for creating list of users from Nebraska*/
    user_node* first_user = (user_node*)malloc(sizeof(user_node));
    search_user_tree(first_Nebraska, first_user);

    key_index = user_binary_search(first_Nebraska, 0, first_user->num_filled-1, first_user);
    
    found_end = 0;
    
    while((key_index >= 0) && (found_end == 0)){
        if(first_user->keys[key_index-1] == first_Nebraska){
            key_index--;
        }
        else{
            found_end=1;
            key_index++;
        }
    }

    user_node* current_user_node = first_user;
	user_table_entry_t* user_struct = (user_table_entry_t *)malloc(sizeof(user_table_entry_t));
    /*Create list of Nebraska Users*/
	int last_user_found = 0;
    while(last_user_found==0 && current_user_node != NULL){
        while(key_index < current_user_node->num_filled){

            if(current_user_node->keys[key_index] <= last_Nebraska){

                sprintf(filename, "users/%s", current_user_node->children[key_index-1]);
                fp = fopen(filename,"rb");
                read_user(fp, user_struct);

                user_current = (user_query_entry *)malloc(sizeof(user_query_entry));

                user_current->user.user_id = user_struct->user_id;

                user_current->next = NULL;

                if (user_head == NULL) {
                  user_head = user_current;
                  user_tail = user_current;
                } else {
                  user_tail->next = user_current;
                  user_tail = user_current;
                }

                key_index++;
            }
            else{
                last_user_found = 1;
                break;
            }
        }

        if(last_user_found == 0){
            read_user_node(current_user_node, current_user_node->children[(current_user_node->num_filled)-1]);
            key_index = 0;
        }
    }

    /*End of creating list of Nebraska Users*/

    /*Beginning logic for finding first timeId */

    //Find first node with correct time key
    time_node* first_time_node = (time_node*)malloc(sizeof(time_node));
    search_time_tree(8.0, first_time_node);

    //Binary search node for time key
    key_index = time_binary_search(8.0, 0, first_time_node->num_filled-1, first_time_node);
	sendtime_table_entry_t* time_struct = (sendtime_table_entry_t *)malloc(sizeof(sendtime_table_entry_t));
    //Access child that has time key and get timeID
    sprintf(filename, "times/%s", first_time_node->children[key_index-1]);
    fp = fopen(filename,"rb");
    read_sendtime(fp, time_struct);
    int first_time = time_struct->sendtime_id;
    fclose(fp);

    /*End of logic for finding first timeID*/

    /*Beginning logic for finding last timeID*/
    search_time_tree(9.0, first_time_node);
    
    //Binary search node for time key
    key_index = time_binary_search(9.0, 0, first_time_node->num_filled-1, first_time_node);

    //Access child that has time key and get timeID
    sprintf(filename, "times/%s", first_time_node->children[key_index-1]);
    fp = fopen(filename,"rb");
    read_sendtime(fp, time_struct);
    int last_time = time_struct->sendtime_id;

    fclose(fp);

    /*End of logic for finding last timeID */

    /*Beginning logic for finding first node with message from Nebraska user*/

    message_node* first_message_node = (message_node*)malloc(sizeof(message_node));
    search_message_tree(first_Nebraska, first_message_node);
    
    //Binary search node for Nebraska key
    key_index = binary_search(first_Nebraska, 0, first_message_node->num_filled-1, first_message_node);

    found_end = 0;

    while((key_index >= 0) && (found_end == 0)){
        if(first_message_node->keys[key_index-1] == first_Nebraska){
            key_index--;
        }
        else{
            found_end=1;
            key_index++;
        }
    }
    /*End logic for finding first node with message from Nebraska User*/

    /*Beginning logic for finding number of Nebraska users who sent messages between 8 and 9*/
    
	int max_count = 0;
	int max_user = 0;

    user_current = user_head;

	message_node* current_message_node = (message_node*)malloc(sizeof(message_node));
	message_table_entry_t* message_struct = (message_table_entry_t*)malloc(sizeof(message_table_entry_t));

    while(user_current != NULL){
		int current_user = user_current->user.user_id;
		int current_count = 0;
        search_message_tree(user_current->user.user_id, current_message_node);

        key_index = binary_search(user_current->user.user_id, 0, current_message_node->num_filled-1, current_message_node);

        while((key_index >= 0) && (found_end == 0)){
            if(current_message_node->keys[key_index-1] == user_current->user.user_id){
                key_index--;
            }
            else{
                found_end=1;
                key_index++;
            }
        }

        found_message = 0;

        while(current_message_node->keys[key_index] < current_message_node->num_filled && current_message_node->keys[key_index]==user_current->user.user_id){

            //Read message and find userId
            sprintf(filename, "messages/%s", current_message_node->children[key_index]);
            fp = fopen(filename,"rb");
            read_message(fp, message_struct);

            fclose(fp);

            if(message_struct->sendtime_id >= first_time && message_struct->sendtime_id <= last_time){
                current_count++;
            }
            key_index++;

            if(key_index == current_user_node->num_filled){
                
                if(current_user_node->children[(current_user_node->num_filled)-1] != NULL){
                    read_message_node(current_message_node, current_message_node->children[(current_message_node->num_filled)-1]);
                    key_index= 0;
                }
                else{
					//reached the end
                    break;
                }

            }

        }
		if (current_count > max_count){
			max_count = current_count;
			max_user = current_user;
		}
        user_current = user_current->next;
    }

    /*End logic for finding number of Nebraska users who sent messages between 8 and 9*/  
    
    /* end time */
    gettimeofday(&time_end, NULL);
    
    float totaltime = (time_end.tv_sec - time_start.tv_sec)
                    + (time_end.tv_usec - time_start.tv_usec) / 1000000.0f;
                    
    free_users(user_head);
    printf("\nUser %d with %d messages", max_user, max_count);
    printf("\n\nProcess time %f seconds\n", totaltime);
    
    free_users(user_head);

    return 0;
}
