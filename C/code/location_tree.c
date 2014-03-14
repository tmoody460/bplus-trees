#include "bp_structs_location.c"
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
void node_insert_and_split(char* old_child, char* new_child, char* key, int index);
void fill_node(node* empty_node);


long node_count = 0;

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
	
	/* Every time we malloc a node, before we read into it
		we call fill_node to avoid having garbage */
	node* leaf_node = (node*) malloc(sizeof(node));
	fill_node(leaf_node);

	/* find leaf we want to insert into */
	get_leaf_for_insert(key, leaf_filename);
	read_node(leaf_node, leaf_filename);

		/* If the leaf isn't full, insert and be done */
	if(leaf_node->num_filled < NUM_KEYS){
		leaf_insert(leaf_filename, key, value);
	} else {
		/* Otherwise split and insert */
		leaf_insert_and_split(leaf_filename, key, value);
	}

	free(leaf_node);
}

void start_tree(char* key, char* value){

	node* root = (node*) malloc(sizeof(node));
	fill_node(root);

	/* Set the initial values of the root with one child */
	strncpy(root->filename, ROOT_PATH, TEXT_SHORT);
	strncpy(root->children[0], value, TEXT_SHORT);
	root->is_leaf = TRUE;
	strncpy(root->keys[0], key, TEXT_SHORT);
	root->num_filled = 1;
	write_node(root);
print_node(root);

	free(root);
}

/* Finds the leaf that the key will be inserted into */
void get_leaf_for_insert(char* key, char* leaf){

	int i, start, end, mid;
	node* current_node = (node*) malloc(sizeof(node));
	fill_node(current_node);
	read_node(current_node, (char*)&ROOT_PATH);

	while(current_node->is_leaf == FALSE){
		int cmp_result;
		/* i becomes the index of the child to look in next */
		i = 0;
		while (i < current_node->num_filled && strcmp(key, current_node->keys[i]) >= 0){
			i++;
		}
		
		/* binary search array of "length" num_filled */
		start = 0;
		end = current_node->num_filled;
		while(end > start) {
			mid = (end - start) / 2;
			cmp_result = strcmp(current_node->keys[mid], key);
			if(cmp_result > 0) {
				end = mid - 1;
			} else if(cmp_result < 0) {
				start = mid + 1;
			} else {
				end = mid;
			}
		}
		cmp_result = strcmp(current_node->keys[start], key);
		if(cmp_result != 0){
			start = current_node->num_filled;
		}
		
		if(i != start) {
			printf("%d %d i != middle !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n", i, start);
		}
		
		//i = start;
		
		
		
		
		read_node(current_node, current_node->children[i]);
	}
	
	/* saves the name of the leaf node that it belongs in */
	strncpy(leaf, current_node->filename, TEXT_SHORT);
	free(current_node);
}

void leaf_insert(char* leaf_filename, char* key, char* value){
	int cmp_result;
	node* leaf_node = (node *) malloc(sizeof(node));
	fill_node(leaf_node);
	read_node(leaf_node, leaf_filename);

	int i, j, start, end, mid;

	/* Get the index of the spot where key should be inserted */
	j = 0;
	while(j < leaf_node->num_filled && strcmp(leaf_node->keys[j], key) < 0){
		j++;
	}
	
	/* binary search array of "length" num_filled */
	start = 0;
	end = leaf_node->num_filled;
	while(end > start) {
		mid = (end - start) / 2;
		cmp_result = strcmp(leaf_node->keys[mid], key);
		if(cmp_result > 0) {
			end = mid - 1;
		} else if(cmp_result < 0) {
			start = mid + 1;
		} else {
			end = mid;
		}
	}
	cmp_result = strcmp(leaf_node->keys[start], key);
	if(cmp_result != 0){
		start = leaf_node->num_filled;
	}
	
	if(j != start) {
		printf("j != middle !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
	}
	
	//j = start;
	

	/* Shift to make space for new key */
	for(i = leaf_node->num_filled - 1; i >= j; i--){
		strncpy(leaf_node->keys[i+1], leaf_node->keys[i], TEXT_SHORT);
		strncpy(leaf_node->children[i+1], leaf_node->children[i], TEXT_SHORT);
	}

	/* Fill in new key */
	strncpy(leaf_node->keys[j], key, TEXT_SHORT);
	strncpy(leaf_node->children[j], value, TEXT_SHORT);
	leaf_node->num_filled++;
	write_node(leaf_node);
print_node(leaf_node);

	free(leaf_node);
}

void leaf_insert_and_split(char* leaf_filename, char* key, char* value){

	/* the node we are inserting into */
	node* leaf_node = (node *) malloc(sizeof(node));
	fill_node(leaf_node);
	read_node(leaf_node, leaf_filename);
	
	int i, j, middle, insert_index;
	char key_for_parent[TEXT_SHORT];
	char filename[TEXT_SHORT];

	char filename_for_parent[TEXT_SHORT];

	char copied_keys[FAN_OUT][TEXT_SHORT];
	char copied_children[FAN_OUT][TEXT_SHORT];

	/* new node to be the sibling */
	node* new_leaf = (node*) malloc(sizeof(node));
	fill_node(new_leaf);
	sprintf(filename, "../data/location_tree/node_%06li.dat", node_count);
	strncpy(new_leaf->filename, filename, TEXT_SHORT);
	node_count++;
	strncpy(new_leaf->parent, "\0", TEXT_SHORT);
	new_leaf->is_leaf = TRUE;
	new_leaf->num_filled = 0;
	
	middle = (NUM_KEYS / 2) + 1;
	
	/* Get the index of the spot where key should be inserted */
	insert_index = 0;
	while(insert_index < leaf_node->num_filled && strcmp(leaf_node->keys[insert_index], key) < 0){
		insert_index++;
	}
	
	/* copy the keys into an array that is one bigger, so it can be split */
	for (i = 0, j = 0; i < leaf_node->num_filled; i++, j++)
	{
		if(j == insert_index){
			j++;
		}
		strncpy(copied_keys[j], leaf_node->keys[i], TEXT_SHORT);
		strncpy(copied_children[j], leaf_node->children[i], TEXT_SHORT);
	}
	strncpy(copied_keys[insert_index], key, TEXT_SHORT);
	strncpy(copied_children[insert_index], value, TEXT_SHORT);

	/* Reset num filled so we can count back up as we add
		the children and keys after the split */
	leaf_node->num_filled = 0;
	for (i = 0; i < middle; i++)
	{
		strncpy(leaf_node->keys[i], copied_keys[i], TEXT_SHORT);
		strncpy(leaf_node->children[i], copied_children[i], TEXT_SHORT);
		leaf_node->num_filled++;
	}
	for (j = 0, i = middle; i < FAN_OUT; j++, i++)
	{
		strncpy(new_leaf->keys[j], copied_keys[i], TEXT_SHORT);
		strncpy(new_leaf->children[j], copied_children[i], TEXT_SHORT);
		new_leaf->num_filled++;
	}
	
	/* the sibling of the new leaf is the sibling of the leaf before the split */
	strncpy(new_leaf->children[FAN_OUT - 1], leaf_node->children[FAN_OUT - 1], TEXT_SHORT);
	
	/* sibling of the old leaf is the new leaf itself */
	strncpy(leaf_node->children[FAN_OUT - 1], new_leaf->filename, TEXT_SHORT);
	
	/* the siblings have the same parent */
	strncpy(new_leaf->parent, leaf_node->parent, TEXT_SHORT);
	
	/* name and child for the parent */
	strncpy(filename_for_parent, leaf_node->filename, TEXT_SHORT);
	strncpy(key_for_parent, new_leaf->keys[0], TEXT_SHORT);

	/* save before insert_key_into_parent because it depends on these nodes */
	write_node(new_leaf);	
	write_node(leaf_node);
print_node(new_leaf);
print_node(leaf_node);

	/* copy the key up into the parent */
	insert_key_into_parent(key_for_parent, leaf_node->filename, new_leaf->filename);
	free(leaf_node);
	free(new_leaf);
}



void insert_key_into_parent(char* key, char* old_child, char* new_child) {

	char filename[TEXT_SHORT];
	int i, j;

	node* old_child_node = (node *) malloc(sizeof(node));
	fill_node(old_child_node);
	read_node(old_child_node, old_child);
	node* new_child_node = (node *) malloc(sizeof(node));
	fill_node(new_child_node);
	read_node(new_child_node, new_child);

	if(strcmp(old_child_node->parent, "\0") == 0){
		/* Make a new root node, and insert key there */
		node* root = (node *) malloc(sizeof(node));
		fill_node(root);
		strncpy(root->filename, ROOT_PATH, TEXT_SHORT);
		root->num_filled = 1;
		strncpy(root->parent, "\0", TEXT_SHORT);
		root->is_leaf = FALSE;
printf("cpy 1\n");
		strncpy(root->keys[0], key, TEXT_SHORT);

		sprintf(filename, "../data/location_tree/node_%06li.dat", node_count);
		strncpy(old_child_node->filename, filename, TEXT_SHORT);
		node_count++;

		strncpy(root->children[0], old_child_node->filename, TEXT_SHORT);
		strncpy(root->children[1], new_child_node->filename, TEXT_SHORT);

		strncpy(old_child_node->parent, ROOT_PATH, TEXT_SHORT);
		strncpy(new_child_node->parent, ROOT_PATH, TEXT_SHORT);
printf("cpy 2\n");
		write_node(root);
		write_node(old_child_node);
		write_node(new_child_node);
//	print_node(root);
//	print_node(old_child_node);
//	print_node(new_child_node);

		node* child_iter = (node *) malloc(sizeof(node));
		fill_node(child_iter);

		if(old_child_node->is_leaf == FALSE){
			for(i = 0; i <= old_child_node->num_filled; i++) {
					read_node(child_iter, old_child_node->children[i]);
					strncpy(child_iter->parent, old_child_node->filename, TEXT_SHORT);
			}
		}

		free(child_iter);
		free(root);
	}else{
		/* Modify existing parent node */

		node* parent_node = (node*) malloc(sizeof(node));
		fill_node(parent_node);
		read_node(parent_node, old_child_node->parent);

		/* Find which child of the parent corresponds to 'old child node' */
		i = 0;
		while(i <= parent_node->num_filled && strcmp(parent_node->children[i], old_child) !=0){
			i++;
		}
		
		/* If parent is not full */
		if(parent_node->num_filled < NUM_KEYS){ 
			/* Go backwards through the children of the parent, shifting children over */
printf("cpy 3\n");			
			for (j = parent_node->num_filled; j > i; j--)
			{
				strncpy(parent_node->keys[j], parent_node->keys[j-1], TEXT_SHORT);
				strncpy(parent_node->children[j+1], parent_node->children[j], TEXT_SHORT);
			}
printf("cpy 4\n");
			strncpy(parent_node->keys[i], key, TEXT_SHORT);
			strncpy(parent_node->children[i+1], new_child, TEXT_SHORT);
			parent_node->num_filled++;
			write_node(parent_node);
//		print_node(parent_node);
		} else {
			node_insert_and_split(parent_node->filename, new_child, key, i);
		}
printf("end\n");
		free(parent_node);
	}

	free(old_child_node);
	free(new_child_node);

}

void node_insert_and_split(char* parent, char* new_child, char* key, int index){

	node* parent_node = (node *) malloc(sizeof(node));
	fill_node(parent_node);
	read_node(parent_node, parent);
	
	node* new_child_node = (node *) malloc(sizeof(node));
	fill_node(new_child_node);
	read_node(new_child_node, new_child);
	
	node* child_iter = (node *) malloc(sizeof(node));
	fill_node(child_iter);

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
		strncpy(copied_keys[j], parent_node->keys[i], TEXT_SHORT);
	}
	for(i = 0, j = 0; i < NUM_KEYS + 1; i++, j++) {
		if(j == index + 1) {
			/* skip the one where we are inserting the key */
			j++;
		} 
		strncpy(copied_children[j], parent_node->children[i], TEXT_SHORT);
	}

	strncpy(copied_keys[index], key, TEXT_SHORT);
	strncpy(copied_children[index + 1], new_child, TEXT_SHORT);

	middle = (FAN_OUT + 1) / 2;

	/* make the new sibling */
	node* new_node = (node*) malloc(sizeof(node));
	fill_node(new_node);
	sprintf(filename, "../data/location_tree/node_%06li.dat", node_count);
	strncpy(new_node->filename, filename, TEXT_SHORT);
	node_count++;
	strncpy(new_node->parent, "\0", TEXT_SHORT);
	new_node->is_leaf = FALSE;
	new_node->num_filled = 0;

	/* parent node is now the left sibling*/
	parent_node->num_filled = 0;
	for(i = 0; i < middle - 1; i++) {
		strncpy(parent_node->children[i], copied_children[i], TEXT_SHORT);
		strncpy(parent_node->keys[i], copied_keys[i], TEXT_SHORT);
		parent_node->num_filled++;
	}
	/* save the middle key to copy to parent,
		don't copy it because it's a leaf node */
	strncpy(parent_node->children[i], copied_children[i], TEXT_SHORT);
	strncpy(key_for_parent, copied_keys[i], TEXT_SHORT);
	
	/* fill the right sibling */
	for(i = middle, j = 0; i < FAN_OUT; i++, j++) {
		strncpy(new_node->children[j], copied_children[i], TEXT_SHORT);
		strncpy(new_node->keys[j], copied_keys[i], TEXT_SHORT);
		new_node->num_filled++;
	}
	strncpy(new_node->children[j], copied_children[i], TEXT_SHORT);
	strncpy(new_node->parent, parent_node->parent, TEXT_SHORT);
	
	write_node(parent_node);
	write_node(new_child_node);
	write_node(new_node);

	/* saves the new name of the parent in its children */
	for(i = 0; i <= new_node->num_filled; i++) {
		read_node(child_iter, new_node->children[i]);
		strncpy(child_iter->parent, new_node->filename, TEXT_SHORT);
		write_node(child_iter);
	}

	insert_key_into_parent(key_for_parent, parent_node->filename, new_node->filename);

	free(child_iter);
	free(new_node);
	free(parent_node);
	free(new_child_node);
}

/* filling the node to avoid having garbage data */
void fill_node(node* empty_node) {
	int i;
	strncpy(empty_node->filename, "\0", TEXT_SHORT);

	for(i = 0; i < FAN_OUT; i++) {
		strncpy(empty_node->children[i], "\0", TEXT_SHORT);
	}	

	strncpy(empty_node->parent, "\0", TEXT_SHORT);
	
	for(i = 0; i < NUM_KEYS; i++) {
		strncpy(empty_node->keys[i], "\0", TEXT_SHORT);
	}

	empty_node->is_leaf = FALSE;
	empty_node->num_filled = 0;
}
