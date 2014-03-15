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

int main(){
	
	location_node* node = (location_node*) malloc(sizeof(location_node));
	
	search_location_tree((char*)&"Nebraska", node);

	printf("stuff %s\n", node->state);
}

void search_user_tree(int query, user_node* current) {
	int i = 0;
	int cmp_result = -1;
	
	read_user_node(current, (char *)&USER_ROOT_PATH);
	int beginning, middle, end;

	int query_found = FALSE;
	
	while(current->is_leaf == FALSE){
		beginning = 0;
		end = current->num_filled - 1;
		while (beginning <= end){
			middle = (beginning + end) / 2;
		}
	}
	
	while(current->is_leaf == FALSE){
		for(i = 0; i < current->num_filled; i++){
			if(current->keys[i] >= query){
				if(current->keys[i] == query){
					query_found = TRUE;
				}
				read_user_node(current, current->children[i]);
				break;
			}
		}
		if (i == current->num_filled){
			read_user_node(current, current->children[i]);
		}
	}
	i=0;
	while(current->keys[i] != query && i < current->num_filled){
		i++;
	}
	if(i == current->num_filled){
		read_user_node(current, current->children[FAN_OUT-1]);
	}
	
	
/*	beginning = 0;
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
	}*/
}

void search_location_tree(char* query, location_node* current) {
	int i = 0;
	int cmp_result = -1;
	
	read_location_node(current, (char *)&LOCATION_ROOT_PATH);

	int beginning, middle, end;
	int cmp_result = 0;
	int exact_match_exists = FALSE;
	while(current->is_leaf == FALSE){
		beginning = 0;
		end = current->num_filled - 1;
		while (beginning <= end){
			middle = (beginning + end) / 2;
			cmp_result = strcmp(current->keys[middle], query);
			if(cmp_result == 0){
				exact_match_exists = TRUE;
			}
			
			if(middle - 1 < 0 && end == 0){
				middle = 0;
				found = TRUE;
			}else if(start == current->num_filled){
				printf("2 if\n");
				middle = start;
				found = TRUE;
			}else if(cmp_result >= 0 
				&& strcmp(current->keys[middle-1], query) < 0){
				printf("3 if\n");
				found = TRUE;
			}else if (cmp_result < 0){
				printf("4 if\n");
				start = middle + 1;
			}else if (cmp_result >= 0){
				printf("5 if\n");
				end = middle - 1;
			}
		}
		read_node(current, current->children[middle]);
	}
	
	
	
	/*int query_found = FALSE;
	while(current->is_leaf == FALSE){
		for(i = 0; i < current->num_filled; i++){
			cmp_result = strcmp(current->keys[i], query);
			if(cmp_result >= 0){
				if(cmp_result == 0){
					query_found = TRUE;
				}
				read_location_node(current, current->children[i]);
				break;
			}
		}
		if (i == current->num_filled){
			read_location_node(current, current->children[i]);
		}
	}
	*/
	
	i=0;
	do{
		cmp_result = strcmp(current->keys[i], query);
		i++;
	}while(cmp_result != 0 && i < current->num_filled);
	i--;
	
	if(i == current->num_filled && exact_match_exists){
		read_location_node(current, current->children[FAN_OUT-1]);
	}
}

void search_message_tree(int q#include "bp_structs.c"
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
	
	read_user_node(current, (char *)&USER_ROOT_PATH);
	int beginning, middle, end;

	int query_found = FALSE;
	
	while(current->is_leaf == FALSE){
		beginning = 0;
		end = current->num_filled - 1;
		while (beginning <= end){
			middle = (beginning + end) / 2;
		}
	}
	
	while(current->is_leaf == FALSE){
		for(i = 0; i < current->num_filled; i++){
			if(current->keys[i] >= query){
				if(current->keys[i] == query){
					query_found = TRUE;
				}
				read_user_node(current, current->children[i]);
				break;
			}
		}
		if (i == current->num_filled){
			read_user_node(current, current->children[i]);
		}
	}
	i=0;
	while(current->keys[i] != query && i < current->num_filled){
		i++;
	}
	if(i == current->num_filled){
		read_user_node(current, current->children[FAN_OUT-1]);
	}
	
	
/*	beginning = 0;
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
	}*/
}

void search_location_tree(char* query, location_node* current) {
	int i = 0;
	int cmp_result = -1;
	int found = FALSE;
	read_location_node(current, (char *)&LOCATION_ROOT_PATH);

/*	int beginning, middle, end;
	int exact_match_exists = FALSE;
	while(current->is_leaf == FALSE){
		beginning = 0;
		end = current->num_filled - 1;
		while (beginning <= end){
			middle = (beginning + end) / 2;
			cmp_result = strcmp(current->keys[middle], query);
			if(cmp_result == 0){
				exact_match_exists = TRUE;
			}
			
			if(middle - 1 < 0 && end == 0){
				middle = 0;
				found = TRUE;
			}else if(beginning == current->num_filled){
				printf("2 if\n");
				middle = beginning;
				found = TRUE;
			}else if(cmp_result >= 0 
				&& strcmp(current->keys[middle-1], query) < 0){
				printf("3 if\n");
				found = TRUE;
			}else if (cmp_result < 0){
				printf("4 if\n");
				beginning = middle + 1;
			}else if (cmp_result >= 0){
				printf("5 if\n");
				end = middle - 1;
			}
		}
		read_location_node(current, current->children[middle]);
	}
*/	
	
	
	int query_found = FALSE;
	while(current->is_leaf == FALSE){
		for(i = 0; i < current->num_filled; i++){
			cmp_result = strcmp(current->keys[i], query);
			if(cmp_result >= 0){
				if(cmp_result == 0){
					query_found = TRUE;
				}
				read_location_node(current, current->children[i]);
				break;
			}
		}
		if (i == current->num_filled){
			read_location_node(current, current->children[i]);
		}
	}
	
	
	i=0;
	do{
		cmp_result = strcmp(current->keys[i], query);
		i++;
	}while(cmp_result != 0 && i < current->num_filled);
	i--;
	
	if(i == current->num_filled && query_found){
		read_location_node(current, current->children[FAN_OUT-1]);
	}
}

void search_message_tree(int query, message_node* current) {
	int i = 0;
	int cmp_result = -1;

	read_message_node(current, (char *)&MESSAGE_ROOT_PATH);
	
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

	read_time_node(current, (char *)&MESSAGE_ROOT_PATH);

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
}uery, message_node* current) {
	int i = 0;
	int cmp_result = -1;

	read_message_node(current, (char *)&MESSAGE_ROOT_PATH);
	
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

	read_time_node(current, (char *)&MESSAGE_ROOT_PATH);

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
