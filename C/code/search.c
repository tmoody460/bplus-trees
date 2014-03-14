#include "bp_structs.c"
#include <string.h>
#include "rw_methods_user.c"
#include "rw_methods_location.c"

void search_location_tree(char* query, location_node* current);
void search_user_tree(char* query, user_node* current);

void search_user_tree(int query, user_node* current) {
	int i = 0;
	int cmp_result = -1;
	
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
	
	int beginning, middle, end;

	int query_found = FALSE;
	while(current->is_leaf == FALSE){
		for(i = 0; i < current->num_filled; i++){
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
