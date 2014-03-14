#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>
#include <string.h>

#include "record.h"
#include "structures.h"
#include "query_helper.h"

//arguments should be in this order:
//number of users
//number of messages
//number of locations
//number of date_times
int nebraska_binary_search(int start, int end, node* current_node);
int binary_search(int key, int start, int end, node* current_node);
int last_binary_search(int key, int start, int end, node current_node)
char* nebraska = "Nebraska";

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

    char first_location = NULL;
    char last_location = NULL;
    int node_not_found = 0;
    int i = 0;

    int found_end = 0;

    int key_index;

    user_table_entry_t* user_head=NULL;
    user_table_entry_t* user_tail;
    user_table_entry_t* user_current;
    user_table_entry_t* previous_user;
    int user_found = 0;

    int found_message;
    int message_count;
    int max_count=0;
    int max_user;

    /* start time */
    gettimeofday(&time_start, NULL);
    //start writing here

    char filename[TEXT_SHORT];

    location_table_entry_t* location_struct;

    FILE *fp = NULL;

    /*Beginning logic for finding first Nebraska locationId */

    //Find first node with Nebraska key
    node* first_node = find_first_Nebraska_node(num_locations, all_locations);

    //Binary search node for Nebraska key
    key_index = nebraska_binary_search(0, first_node->num_filled, first_node);

    //Binary search will not necessarily give the first Nebraska key 
    while((key_index >= 0) && (found_end == 0)){
        if(strcmp(first_node->keys[key_index-1], nebraska) != NULL){
            key_index--;
        }
        else{
            found_end=1;
            key_index++;
        }
    }

    //Access child that has Nebraska key and get locationID
    sprintf(filename, "locations/%c", first_node->children[key_index-1]);
    fp = fopen(filename,"rb");
    read_location(fp, location_struct);
    int first_Nebraska = location_struct->location_id;
    fpclose(fp);

    /*End of logic for finding first Nebraska locationId*/

    /*Beginning logic for finding last Nebraska locationId */

    node* current_node = first_node;
    node* previous_node = NULL;
    
    found_end = 0;
    key_index = NULL;
    while(current_node != NULL && found_end == 0){
        
        int comparison = strcmp(current_node->keys[(current_node->num_filled)-2], nebraska)

        if(comparison == 0){
            previous_node = current_node;
            if(current_node->keys[fan_out-1] != NULL){
                read_node(current_node, current_node->keys[fan_out-1]);
            }
            else{
                current_node = NULL;
            }

        }
        else if(comparison > 0){
            key_index = nebraska_binary_search(0, current_node->num_filled, current_node);
            
            //Binary search will not necessarily give the last Nebraska key 
            while((key_index < current_node->num_filled) && (found_end == 0)){
                if(strcmp(current_node->keys[key_index+1], nebraska) != NULL){
                    key_index++;
                }
                else{
                    found_end=1;
                }
            }
        }        

    }

    if(key_index == NULL){
        current_node = previous_node;
        key_index = current_node->num_filled;
    }

    sprintf(filename, "locations/%c", current_node->children[key_index-1]);
    fp = fopen(filename,"rb");
    read_location(fp, location_struct);
    int last_Nebraska = location_struct->location_id;

    fpclose(fp);

    /*End of logic for finding last Nebraska locationId */
    
    
    /*Beginning logic for creating list of users from Nebraska*/
    node* first_user = find_user(first_Nebraska);

    key_index = binary_search(first_Nebraska, 0, first_user->num_filled, first_user);
    
    found_end = 0;
    
    while((key_index >= 0) && (found_end == 0)){
        if(strcmp(first_node->keys[key_index-1], nebraska) != NULL){
            key_index--;
        }
        else{
            found_end=1;
            key_index++;
        }
    }

    current_node = first_user;

    /*Create list of Nebraska Users*/

    while(last_user_found==0 && current_node != NULL){
        while(key_index < current_node->num_filled){

            if(current_node->keys[key_index] <= last_Nebraska){

                sprintf(filename, "users/%c", current_node->children[key_index-1]);
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
            read_node(current_node, current_node->children[(current_node->num_filled)-1]);
            key_index = 0;
        }
    }

    /*End of creating list of Nebraska Users*/

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

    /*Beginning logic for finding first node with message from Nebraska user*/

    node* first_node = find_first_time_node(first_Nebraska);
    
    //Binary search node for Nebraska key
    key_index = nebraska_binary_search(0, first_node->num_filled, first_node);

    found_end = 0;

    while((key_index >= 0) && (found_end == 0)){
        if(strcmp(first_node->keys[key_index-1], nebraska) != NULL){
            key_index--;
        }
        else{
            found_end=1;
            key_index++;
        }
    }
    /*End logic for finding first node with message sent between 8 and 9*/

    /*Beginning logic for finding max number of messages sent between 8 and 9 from Nebraska users */
    
    user_current = user_head;

    while(user_current ! = NULL){

        first_node = find_first_message(user_current->user.user_id);

        key_index = binary_search(user_current->user.user_id, 0, first_user->num_filled, first_user);

        while((key_index >= 0) && (found_end == 0)){
            if(first_node->keys[key_index-1] == user_current->user.user_id){
                key_index--;
            }
            else{
                found_end=1;
                key_index++;
            }
        }

        message_count = 0;

        while(current_node->keys[key_index] < num_filled && current_node->keys[key_index]==user_current->user.user_id){

            //Read message and find userId
            sprintf(filename, "messages/%c", current_node->children[key_index]);
            fp = fopen(filename,"rb");
            read_message(fp, message_struct);

            if(message_struct->time_id >= first_time && message_struct->time_id <= last_time){
                message_count++;
            }
            key_index++;

            if(key_index == current_node->num_filled){
                
                if(current_node->children[(current_node->num_filled)-1] != NULL){
                    read(current_node, current_node->children[(current_node->num_filled)-1]);
                    key_index= 0;
                }
                else{
                    break;
                }

            }

        }

        if(max_count < message_count){
            max_user = user_current->user.user_id;
            max_count = message_count;
        }

        user_current = user_current->next;
    }

    /*End logic for finding max number of messages sent between 8 and 9 from Nebraska users */
    
    /* end time */
    gettimeofday(&time_end, NULL);
    
    float totaltime = (time_end.tv_sec - time_start.tv_sec)
                    + (time_end.tv_usec - time_start.tv_usec) / 1000000.0f;
                    
    free_locations(location_head);
    printf("\nThere are %d users from Nebraska.", user_count);
    printf("\n\nProcess time %f seconds\n", totaltime);
    
    free_users(user_head);

    return 0;
}

int nebraska_binary_search(int start, int end, node* current_node){

    int middle = (start + end)/2;

    int comparison = strcmp(current_node->keys[middle], nebraska);

    if(comparison == 0)
    {
        return current_node->children[middle];
    }
    else{
        return binarysearch(key, start, middle, current_node);
    }
   
   //if it is not found in the whole array
   return -1;

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