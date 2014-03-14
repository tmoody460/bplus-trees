#include "bp_structs.c"
#include <string.h>
#include "rw_methods_user.c"
#include "rw_methods_location.c"
#include "rw_methods_message.c"
#include "rw_methods_time.c"

void search_location_tree(char* query, location_node* current);
void search_user_tree(int query, user_node* current);
void search_message_tree(int query, message_node* current);
void search_time_tree(float query, time_node* current);

void search_user_tree(int query, user_node* current) {
	int i = 0;
	int cmp_result = -1;
	
	read_user_node(current, USER_ROOT_PATH);
	int beginning, middle, end;

	int query_found = FALSE;
	while(current->is_leaf == FALSE){
		for(i = 0; i < current->num_filled; i++){
			if(current->keys[i] < query){
				read_user_node(current, current->children[i]);
			}else if(current->keys[i] == query){
				query_found = TRUE;
			}
		}
		if (i == current->num_filled){
			read_user_node(current, current->children[i]);
		}
	}
	
	beginning = 0;
	end = current->num_filled - 1;
	while(beginning < end){
		middle = (beginning + end) / 2;
		if (current->keys[middle] > query){
			end = middle - 1;
		}else if (current->keys[middle] < query){
			beginning = middle + 1;
		}else{
			end = middle;
		}
	}
	if(current->keys[beginning] != query){
		if(query_found){
			read_user_node(current, current->children[FAN_OUT-1]);
		}else{
			current = NULL;
		}
	}
}

void search_location_tree(char* query, location_node* current) {
	int i = 0;
	int cmp_result = -1;
	
	read_location_node(current, LOCATION_ROOT_PATH);

	int beginning, middle, end;

	int query_found = FALSE;
	while(current->is_leaf == FALSE){
		for(i = 0; i < current->num_filled; i++){
			printf("notehu\n");
			cmp_result = strcmp(current->keys[i], query);
			if(cmp_result < 0){
				read_location_node(current, current->children[i]);
			}else if(cmp_result == 0){
				query_found = TRUE;
			}
		}
		if (i == current->num_filled){
			read_location_node(current, current->children[i]);
		}
	}
	
	beginning = 0;
	end = current->num_filled - 1;
	while(beginning < end){
		middle = (beginning + end) / 2;
		cmp_result = strcmp(current->keys[middle], query);
		if (cmp_result > 0){
			end = middle - 1;
		}else if (cmp_result < 0){
			beginning = middle + 1;
		}else{
			end = middle;
		}
	}
	if(strcmp(current->keys[middle], query) != 0){
		if(query_found){
			read_location_node(current, current->children[FAN_OUT-1]);
		}else{
			current = NULL;
		}
	}
}

void search_message_tree(int query, message_node* current) {
	int i = 0;
	int cmp_result = -1;

	read_message_node(current, MESSAGE_ROOT_PATH);
	
	int beginning, middle, end;

	int query_found = FALSE;
	while(current->is_leaf == FALSE){
		for(i = 0; i < current->num_filled; i++){
			if(current->keys[i] < query){
				read_message_node(current, current->children[i]);
			}else if(current->keys[i] == query){
				query_found = TRUE;
			}
		}
		if (i == current->num_filled){
			read_message_node(current, current->children[i]);
		}
	}
	
	beginning = 0;
	end = current->num_filled - 1;
	while(beginning < end){
		middle = (beginning + end) / 2;
		if (current->keys[middle] > query){
			end = middle - 1;
		}else if (current->keys[middle] < query){
			beginning = middle + 1;
		}else{
			end = middle;
		}
	}
	if(current->keys[beginning] != query){
		if(query_found){
			read_message_node(current, current->children[FAN_OUT-1]);
		}else{
			current = NULL;
		}
	}
}

void search_time_tree(float query, time_node* current) {
	int i = 0;
	int cmp_result = -1;

	read_message_node(current, MESSAGE_ROOT_PATH);

	int beginning, middle, end;

	int query_found = FALSE;
	while(current->is_leaf == FALSE){
		for(i = 0; i < current->num_filled; i++){
			if(current->keys[i] < query){
				read_time_node(current, current->children[i]);
			}else if(current->keys[i] == query){
				query_found = TRUE;
			}
		}
		if (i == current->num_filled){
			read_time_node(current, current->children[i]);
		}
	}
	
	beginning = 0;
	end = current->num_filled - 1;
	while(beginning < end){
		middle = (beginning + end) / 2;
		if (current->keys[middle] > query){
			end = middle - 1;
		}else if (current->keys[middle] < query){
			beginning = middle + 1;
		}else{
			end = middle;
		}
	}
	if(current->keys[beginning] != query){
		if(query_found){
			read_time_node(current, current->children[FAN_OUT-1]);
		}else{
			current = NULL;
		}
	}
}
