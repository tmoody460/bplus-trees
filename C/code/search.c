#include "bp_structs.c"
#include <string.h>
#include "rw_methods.c"

node* search_location_tree(char* query){
	int i = 0;
	node* current = (node *) malloc(sizeof(node));
	read_node(current, ROOT_PATH);
	int cmp_result = -1;
	
	int beginning, middle, end;

	int query_found = FALSE;
	while(current->is_leaf == FALSE){
		for(i = 0; i < current->num_filled; i++){
			cmp_result = strcmp(current->keys[i], query);
			if(cmp_result < 0){
				read_node(current, current->children[i]);
			}else if(cmp_result == 0){
				query_found = TRUE;
			}
		}
		if (i == current->num_filled){
			read_node(current, current->children[i]);
		}
	}
	
	beginning = 0;
	end = current->num_filled - 1;
	while(beginning < end){
		middle = (beginning + end) / 2;
		cmp_result = strcmp(current->keys[middle], query));
		if (cmp_result > 0){
			end = middle - 1;
		}else if (cmp_result < 0){
			beginning = middle + 1;
		}else{
			end = middle;
		}
	}
	if(strcmp(current->keys[middle], query)) != 0){
		if(query_found){
			read_node(current, current[FAN_OUT-1]);
		}else{
			current = NULL;
		}
	}
	
	return current;
}