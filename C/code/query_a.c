#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>
#include <sys/time.h>
#include <string.h>

#include "tables.h"
#include "query_helper.h"
#include "bp_structs.c"
#include "search.c"


//arguments should be in this order:
//number of users
//number of messages
//number of locations
//number of date_times
int nebraska_binary_search(int start, int end, location_node* current_node);
int binary_search(int key, int start, int end, user_node* current_node);
int last_binary_search(int key, int start, int end, user_node* current_node);

char* nebraska = "Nebraska";

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

    /* start time */
    gettimeofday(&time_start, NULL);
    //start writing here

    char filename[TEXT_SHORT];

    location_table_entry_t* location_struct;

    FILE *fp = NULL;

    /*Beginning logic for finding first Nebraska locationId */

    //Find first node with Nebraska key
    location_node* first_node = (location_node *)malloc(sizeof(location_node));
	search_location_tree(nebraska, first_node);

    //Binary search node for Nebraska key
    key_index = nebraska_binary_search(0, first_node->num_filled, first_node);

    //Binary search will not necessarily give the first Nebraska key 
    while((key_index >= 0) && (found_end == 0)){
        if(strstr(first_node->keys[key_index-1], nebraska) != NULL){
            key_index--;
        }
        else{
            found_end=1;
            key_index++;
        }
    }

    //Access child that has Nebraska key and get locationID
    sprintf(filename, "locations/%s", first_node->children[key_index-1]);
    fp = fopen(filename,"rb");
    read_location(fp, location_struct);
    int first_Nebraska = location_struct->location_id;
    fclose(fp);

    /*End of logic for finding first Nebraska locationId*/

    /*Beginning logic for finding last Nebraska locationId */

    location_node* current_node = first_node;
    location_node* previous_node = NULL;
    
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
            key_index = nebraska_binary_search(0, current_node->num_filled, current_node);
            
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
    
    
    /*Beginning logic for finding first user from Nebraska*/

    user_node* first_user_node = (user_node *)malloc(sizeof(user_node));
	user_node* current_user_node = NULL;
	search_user_tree(first_Nebraska, first_user_node);

    key_index = binary_search(first_Nebraska, 0, first_user_node->num_filled, first_user_node);

    //Binary search will not necessarily give the last location key 
    while((key_index >= 0) && (found_end == 0)){
        if(first_user_node->keys[key_index-1] == first_Nebraska){
            key_index--;
        }
        else{
            found_end=1;
            key_index++;
        }
    }

    /*End of logic for finding first user from Nebraska*/

    user_count = (FAN_OUT - key_index) + 1;

    /*Beginning logic for finding last user from Nebraska */

    current_user_node = first_user_node;
    user_node* previous_user_node = NULL;
    found_end = 0;
    i = 0;
    key_index = -1;
    while(current_user_node != NULL && found_end == 0){

        if(current_user_node->keys[(current_user_node->num_filled)-2] <= last_Nebraska){
            previous_user_node = current_user_node;
            if(current_user_node->keys[FAN_OUT-1] != NULL){
                read_node(current_user_node, current_user_node->keys[FAN_OUT-1]);
                i++
            }
            else{
                current_user_node = NULL;
            }
        }
        else if(current_user_node->keys[(current_node->num_filled)-2] > last_Nebraska){
            key_index = last_binary_search(last_Nebraska, 0, current_user_node->num_filled, current_user_node);
            
            //Binary search will not necessarily give the last Nebraska key 
            while((key_index < current_user_node->num_filled) && (found_end == 0)){
                if(current_user_node->keys[(current_user_node->num_filled)-2] <= last_Nebraska){
                    key_index++;
                }
                else{
                    found_end=1;
                }
            }
        }        

    }

    if(key_index == -1){
        key_index = previous_user_node->num_filled;
    }
    /*End of logic for finding last user from Nebraska */
    
    user_count = user_count + (i*FAN_OUT) + key_index;

    //stop writing here    
    
    /* end time */
    gettimeofday(&time_end, NULL);
    
    float totaltime = (time_end.tv_sec - time_start.tv_sec)
                    + (time_end.tv_usec - time_start.tv_usec) / 1000000.0f;
    free(first_node);
	free(first_user_node);                
    free_locations(location_head);
    printf("\nThere are %d users from Nebraska.", user_count);
    printf("\n\nProcess time %f seconds\n", totaltime);
    
    return 0;
}

int nebraska_binary_search(int start, int end, node* current_node){

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
