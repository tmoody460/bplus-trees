#include "bp_structs.c"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "tables.h"
#include "query_helper.h"
#include "rw_methods_location.c"

void insert(char* key, char* value);
void start_tree(char* key, char* value);
void get_leaf_for_insert(char* key, char* leaf);
void leaf_insert(char* leaf_filename, char* key, char* value);
void leaf_insert_and_split(char* leaf_filename, char* key, char* value);
void insert_key_into_parent(char* key, char* old_child, char* new_child);
void location_node_insert_and_split(char* old_child, char* new_child, char* key, int index);
void fill_location_node(location_node* empty_location_node);


long location_node_count = 0;

int main(int argc, char** argv) {

	if (argc != 2) {
        fprintf(stderr, "Usage: %s #locations\n", argv[0]);
        exit(0);
    }

	int i;
	char filename[TEXT_SHORT];
	int num_records = atoi(argv[1]);

	location_table_entry_t* current_location;
	current_location = (location_table_entry_t *) malloc(sizeof(location_table_entry_t));

	/* read the first location */
	if(num_records > 0){
		read_location_better(0, current_location);
		sprintf(filename, "locations/location_%06d.dat", 0);
		start_tree(current_location->state, filename);
	}

	/* read the rest of the locations */
	for(i = 1; i < num_records; i++) {
		read_location_better(i, current_location);
		sprintf(filename, "locations/location_%06d.dat", i);
		insert(current_location->state, filename);
	}

	free(current_location);
	return 0;
}

void insert(char* key, char* value) {

	char leaf_filename[TEXT_SHORT];
	
	/* Every time we malloc a location_node, before we read into it
		we call fill_location_node to avoid having garbage */
	location_node* leaf_location_node = (location_node*) malloc(sizeof(location_node));
	fill_location_node(leaf_location_node);

	/* find leaf we want to insert into */
	get_leaf_for_insert(key, leaf_filename);
	read_location_node(leaf_location_node, leaf_filename);

		/* If the leaf isn't full, insert and be done */
	if(leaf_location_node->num_filled < NUM_KEYS){
		leaf_insert(leaf_filename, key, value);
	} else {
		/* Otherwise split and insert */
		leaf_insert_and_split(leaf_filename, key, value);
	}

	free(leaf_location_node);
}

void start_tree(char* key, char* value){

	location_node* root = (location_node*) malloc(sizeof(location_node));
	fill_location_node(root);

	/* Set the initial values of the root with one child */
	strncpy(root->filename, LOCATION_ROOT_PATH, TEXT_SHORT);
	strncpy(root->children[0], value, TEXT_SHORT);
	root->is_leaf = TRUE;
	strncpy(root->keys[0], key, TEXT_SHORT);
	root->num_filled = 1;
	write_location_node(root);
print_location_node(root);

	free(root);
}

/* Finds the leaf that the key will be inserted into */
void get_leaf_for_insert(char* key, char* leaf){

	int i, start, end, middle;
	location_node* current_location_node = (location_node*) malloc(sizeof(location_node));
	fill_location_node(current_location_node);
	read_location_node(current_location_node, (char*)&LOCATION_ROOT_PATH);

	while(current_location_node->is_leaf == FALSE){
		int cmp_result;
		/* i becomes the index of the child to look in next */
		i = 0;
		while (i < current_location_node->num_filled && strcmp(key, current_location_node->keys[i]) >= 0){
			i++;
		}
		
		/* binary search array of "length" num_filled */
		start = 0;
		end = current_location_node->num_filled;
		int found = 0;
		while(end >= start && found ==FALSE){
			middle = (start + end)/ 2;
printf("Comparing %s with %s\n", current_location_node->keys[middle], key );
			cmp_result = strcmp(current_location_node->keys[middle], key);
			if(middle - 1 < 0 && end == 0){
			printf("first if\n");
				middle = 0;
				found = TRUE;
			}else if(start == current_location_node->num_filled){
				printf("2 if\n");
				middle = start;
				found = TRUE;
			}else if(cmp_result > 0 
				&& strcmp(current_location_node->keys[middle-1], key) <= 0){
				printf("3 if\n");
				found = TRUE;
			}else if (cmp_result <= 0){
				printf("4 if\n");
				start = middle + 1;
			}else if (cmp_result > 0){
				printf("5 if\n");
				end = middle - 1;
			}
		}
		
		if(i != middle) {
			printf("i: %d  middle: %d !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n", i, middle);
		}else{
			printf("It worked\n");
		}
		
		//i = start;
		
		
		
		
		read_location_node(current_location_node, current_location_node->children[i]);
	}
	
	/* saves the name of the leaf location_node that it belongs in */
	strncpy(leaf, current_location_node->filename, TEXT_SHORT);
	free(current_location_node);
}

void leaf_insert(char* leaf_filename, char* key, char* value){
	int cmp_result;
	location_node* leaf_location_node = (location_node *) malloc(sizeof(location_node));
	fill_location_node(leaf_location_node);
	read_location_node(leaf_location_node, leaf_filename);

	int i, j, start, end, middle;

	/* Get the index of the spot where key should be inserted */
	j = 0;
	while(j < leaf_location_node->num_filled && strcmp(leaf_location_node->keys[j], key) < 0){
		j++;
	}
	
	/* binary search array of "length" num_filled */
	start = 0;
	end = leaf_location_node->num_filled;
	int found = 0;
	while(end >= start && found ==FALSE){
		middle = (start + end)/ 2;
printf("Comparing %s with %s\n", leaf_location_node->keys[middle], key );
		cmp_result = strcmp(leaf_location_node->keys[middle], key);
		if(middle - 1 < 0 && end == 0){
		printf("first if\n");
			middle = 0;
			found = TRUE;
		}else if(start == leaf_location_node->num_filled){
			printf("2 if\n");
			middle = start;
			found = TRUE;
		}else if(cmp_result >= 0 
			&& strcmp(leaf_location_node->keys[middle-1], key) < 0){
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

	if(j != middle) {
		printf("j: %d  middle: %d !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n", j, middle);
	}else{
		printf("It worked\n");
	}
	
	//j = start;
	

	/* Shift to make space for new key */
	for(i = leaf_location_node->num_filled - 1; i >= j; i--){
		strncpy(leaf_location_node->keys[i+1], leaf_location_node->keys[i], TEXT_SHORT);
		strncpy(leaf_location_node->children[i+1], leaf_location_node->children[i], TEXT_SHORT);
	}

	/* Fill in new key */
	strncpy(leaf_location_node->keys[j], key, TEXT_SHORT);
	strncpy(leaf_location_node->children[j], value, TEXT_SHORT);
	leaf_location_node->num_filled++;
	write_location_node(leaf_location_node);
print_location_node(leaf_location_node);

	free(leaf_location_node);
}

void leaf_insert_and_split(char* leaf_filename, char* key, char* value){

	/* the location_node we are inserting into */
	location_node* leaf_location_node = (location_node *) malloc(sizeof(location_node));
	fill_location_node(leaf_location_node);
	read_location_node(leaf_location_node, leaf_filename);
	
	int i, j, middle, insert_index;
	char key_for_parent[TEXT_SHORT];
	char filename[TEXT_SHORT];

	char filename_for_parent[TEXT_SHORT];

	char copied_keys[FAN_OUT][TEXT_SHORT];
	char copied_children[FAN_OUT][TEXT_SHORT];

	/* new location_node to be the sibling */
	location_node* new_leaf = (location_node*) malloc(sizeof(location_node));
	fill_location_node(new_leaf);
	sprintf(filename, "../data/location_tree/location_node_%06li.dat", location_node_count);
	strncpy(new_leaf->filename, filename, TEXT_SHORT);
	location_node_count++;
	strncpy(new_leaf->parent, "\0", TEXT_SHORT);
	new_leaf->is_leaf = TRUE;
	new_leaf->num_filled = 0;
	
	middle = (NUM_KEYS / 2) + 1;
	
	/* Get the index of the spot where key should be inserted */
	insert_index = 0;
	while(insert_index < leaf_location_node->num_filled && strcmp(leaf_location_node->keys[insert_index], key) < 0){
		insert_index++;
	}
	
	/* copy the keys into an array that is one bigger, so it can be split */
	for (i = 0, j = 0; i < leaf_location_node->num_filled; i++, j++)
	{
		if(j == insert_index){
			j++;
		}
		strncpy(copied_keys[j], leaf_location_node->keys[i], TEXT_SHORT);
		strncpy(copied_children[j], leaf_location_node->children[i], TEXT_SHORT);
	}
	strncpy(copied_keys[insert_index], key, TEXT_SHORT);
	strncpy(copied_children[insert_index], value, TEXT_SHORT);

	/* Reset num filled so we can count back up as we add
		the children and keys after the split */
	leaf_location_node->num_filled = 0;
	for (i = 0; i < middle; i++)
	{
		strncpy(leaf_location_node->keys[i], copied_keys[i], TEXT_SHORT);
		strncpy(leaf_location_node->children[i], copied_children[i], TEXT_SHORT);
		leaf_location_node->num_filled++;
	}
	for (j = 0, i = middle; i < FAN_OUT; j++, i++)
	{
		strncpy(new_leaf->keys[j], copied_keys[i], TEXT_SHORT);
		strncpy(new_leaf->children[j], copied_children[i], TEXT_SHORT);
		new_leaf->num_filled++;
	}
	
	/* the sibling of the new leaf is the sibling of the leaf before the split */
	strncpy(new_leaf->children[FAN_OUT - 1], leaf_location_node->children[FAN_OUT - 1], TEXT_SHORT);
	
	/* sibling of the old leaf is the new leaf itself */
	strncpy(leaf_location_node->children[FAN_OUT - 1], new_leaf->filename, TEXT_SHORT);
	
	/* the siblings have the same parent */
	strncpy(new_leaf->parent, leaf_location_node->parent, TEXT_SHORT);
	
	/* name and child for the parent */
	strncpy(filename_for_parent, leaf_location_node->filename, TEXT_SHORT);
	strncpy(key_for_parent, new_leaf->keys[0], TEXT_SHORT);

	/* save before insert_key_into_parent because it depends on these location_nodes */
	write_location_node(new_leaf);	
	write_location_node(leaf_location_node);
print_location_node(new_leaf);
print_location_node(leaf_location_node);

	/* copy the key up into the parent */
	insert_key_into_parent(key_for_parent, leaf_location_node->filename, new_leaf->filename);
	free(leaf_location_node);
	free(new_leaf);
}



void insert_key_into_parent(char* key, char* old_child, char* new_child) {

	char filename[TEXT_SHORT];
	int i, j;

	location_node* old_child_location_node = (location_node *) malloc(sizeof(location_node));
	fill_location_node(old_child_location_node);
	read_location_node(old_child_location_node, old_child);
	location_node* new_child_location_node = (location_node *) malloc(sizeof(location_node));
	fill_location_node(new_child_location_node);
	read_location_node(new_child_location_node, new_child);

	if(strcmp(old_child_location_node->parent, "\0") == 0){
		/* Make a new root location_node, and insert key there */
		location_node* root = (location_node *) malloc(sizeof(location_node));
		fill_location_node(root);
		strncpy(root->filename, LOCATION_ROOT_PATH, TEXT_SHORT);
		root->num_filled = 1;
		strncpy(root->parent, "\0", TEXT_SHORT);
		root->is_leaf = FALSE;
printf("cpy 1\n");
		strncpy(root->keys[0], key, TEXT_SHORT);

		sprintf(filename, "../data/location_tree/location_node_%06li.dat", location_node_count);
		strncpy(old_child_location_node->filename, filename, TEXT_SHORT);
		location_node_count++;

		strncpy(root->children[0], old_child_location_node->filename, TEXT_SHORT);
		strncpy(root->children[1], new_child_location_node->filename, TEXT_SHORT);

		strncpy(old_child_location_node->parent, LOCATION_ROOT_PATH, TEXT_SHORT);
		strncpy(new_child_location_node->parent, LOCATION_ROOT_PATH, TEXT_SHORT);
printf("cpy 2\n");
		write_location_node(root);
		write_location_node(old_child_location_node);
		write_location_node(new_child_location_node);
//	print_location_node(root);
//	print_location_node(old_child_location_node);
//	print_location_node(new_child_location_node);

		location_node* child_iter = (location_node *) malloc(sizeof(location_node));
		fill_location_node(child_iter);

		if(old_child_location_node->is_leaf == FALSE){
			for(i = 0; i <= old_child_location_node->num_filled; i++) {
					read_location_node(child_iter, old_child_location_node->children[i]);
					strncpy(child_iter->parent, old_child_location_node->filename, TEXT_SHORT);
			}
		}

		free(child_iter);
		free(root);
	}else{
		/* Modify existing parent location_node */

		location_node* parent_location_node = (location_node*) malloc(sizeof(location_node));
		fill_location_node(parent_location_node);
		read_location_node(parent_location_node, old_child_location_node->parent);

		/* Find which child of the parent corresponds to 'old child location_node' */
		i = 0;
		while(i <= parent_location_node->num_filled && strcmp(parent_location_node->children[i], old_child) !=0){
			i++;
		}
		
		/* If parent is not full */
		if(parent_location_node->num_filled < NUM_KEYS){ 
			/* Go backwards through the children of the parent, shifting children over */
printf("cpy 3\n");			
			for (j = parent_location_node->num_filled; j > i; j--)
			{
				strncpy(parent_location_node->keys[j], parent_location_node->keys[j-1], TEXT_SHORT);
				strncpy(parent_location_node->children[j+1], parent_location_node->children[j], TEXT_SHORT);
			}
printf("cpy 4\n");
			strncpy(parent_location_node->keys[i], key, TEXT_SHORT);
			strncpy(parent_location_node->children[i+1], new_child, TEXT_SHORT);
			parent_location_node->num_filled++;
			write_location_node(parent_location_node);
//		print_location_node(parent_location_node);
		} else {
			location_node_insert_and_split(parent_location_node->filename, new_child, key, i);
		}
printf("end\n");
		free(parent_location_node);
	}

	free(old_child_location_node);
	free(new_child_location_node);

}

void location_node_insert_and_split(char* parent, char* new_child, char* key, int index){

	location_node* parent_location_node = (location_node *) malloc(sizeof(location_node));
	fill_location_node(parent_location_node);
	read_location_node(parent_location_node, parent);
	
	location_node* new_child_location_node = (location_node *) malloc(sizeof(location_node));
	fill_location_node(new_child_location_node);
	read_location_node(new_child_location_node, new_child);
	
	location_node* child_iter = (location_node *) malloc(sizeof(location_node));
	fill_location_node(child_iter);

	char filename[TEXT_SHORT];
	char copied_keys[FAN_OUT][TEXT_SHORT];
	char copied_children[FAN_OUT+1][TEXT_SHORT];
	int i, j, middle;
	char key_for_parent[TEXT_SHORT];
	
	/* copy the children and keys into an array that is one bigger
		so we can split them */	
	for(i = 0, j = 0; i < NUM_KEYS; i++, j++) {
		if(j == index) {
			/* skip the one where we are inserting the key */
			j++;
		}
		strncpy(copied_keys[j], parent_location_node->keys[i], TEXT_SHORT);
	}
	for(i = 0, j = 0; i < NUM_KEYS + 1; i++, j++) {
		if(j == index + 1) {
			/* skip the one where we are inserting the key */
			j++;
		} 
		strncpy(copied_children[j], parent_location_node->children[i], TEXT_SHORT);
	}

	strncpy(copied_keys[index], key, TEXT_SHORT);
	strncpy(copied_children[index + 1], new_child, TEXT_SHORT);

	middle = (FAN_OUT + 1) / 2;

	/* make the new sibling */
	location_node* new_location_node = (location_node*) malloc(sizeof(location_node));
	fill_location_node(new_location_node);
	sprintf(filename, "../data/location_tree/location_node_%06li.dat", location_node_count);
	strncpy(new_location_node->filename, filename, TEXT_SHORT);
	location_node_count++;
	strncpy(new_location_node->parent, "\0", TEXT_SHORT);
	new_location_node->is_leaf = FALSE;
	new_location_node->num_filled = 0;

	/* parent location_node is now the left sibling*/
	parent_location_node->num_filled = 0;
	for(i = 0; i < middle - 1; i++) {
		strncpy(parent_location_node->children[i], copied_children[i], TEXT_SHORT);
		strncpy(parent_location_node->keys[i], copied_keys[i], TEXT_SHORT);
		parent_location_node->num_filled++;
	}
	/* save the middle key to copy to parent,
		don't copy it because it's a leaf location_node */
	strncpy(parent_location_node->children[i], copied_children[i], TEXT_SHORT);
	strncpy(key_for_parent, copied_keys[i], TEXT_SHORT);
	
	/* fill the right sibling */
	for(i = middle, j = 0; i < FAN_OUT; i++, j++) {
		strncpy(new_location_node->children[j], copied_children[i], TEXT_SHORT);
		strncpy(new_location_node->keys[j], copied_keys[i], TEXT_SHORT);
		new_location_node->num_filled++;
	}
	strncpy(new_location_node->children[j], copied_children[i], TEXT_SHORT);
	strncpy(new_location_node->parent, parent_location_node->parent, TEXT_SHORT);
	
	write_location_node(parent_location_node);
	write_location_node(new_child_location_node);
	write_location_node(new_location_node);

	/* saves the new name of the parent in its children */
	for(i = 0; i <= new_location_node->num_filled; i++) {
		read_location_node(child_iter, new_location_node->children[i]);
		strncpy(child_iter->parent, new_location_node->filename, TEXT_SHORT);
		write_location_node(child_iter);
	}

	insert_key_into_parent(key_for_parent, parent_location_node->filename, new_location_node->filename);

	free(child_iter);
	free(new_location_node);
	free(parent_location_node);
	free(new_child_location_node);
}

/* filling the location_node to avoid having garbage data */
void fill_location_node(location_node* empty_location_node) {
	int i;
	strncpy(empty_location_node->filename, "\0", TEXT_SHORT);

	for(i = 0; i < FAN_OUT; i++) {
		strncpy(empty_location_node->children[i], "\0", TEXT_SHORT);
	}	

	strncpy(empty_location_node->parent, "\0", TEXT_SHORT);
	
	for(i = 0; i < NUM_KEYS; i++) {
		strncpy(empty_location_node->keys[i], "\0", TEXT_SHORT);
	}

	empty_location_node->is_leaf = FALSE;
	empty_location_node->num_filled = 0;
}
