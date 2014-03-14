#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "tables.h"
#include "query_helper.h"
#include "rw_methods_user.c"

void insert(int key, char* value);
void start_tree(int key, char* value);
void get_leaf_for_insert(int key, char* leaf);
void leaf_insert(char* leaf_filename, int key, char* value);
void leaf_insert_and_split(char* leaf_filename, int key, char* value);
void insert_key_into_parent(int key, char* old_child, char* new_child);
void user_node_insert_and_split(char* old_child, char* new_child, int key, int index);
void fill_user_node(user_node* empty_user_node);


long user_node_count = 0;

int main(int argc, char** argv) {

	if (argc != 2) {
        fprintf(stderr, "Usage: %s #users\n", argv[0]);
        exit(0);
    }

	int i;
	char filename[TEXT_SHORT];
	int num_records = atoi(argv[1]);

	user_table_entry_t* current_user;
	current_user = (user_table_entry_t *) malloc(sizeof(user_table_entry_t));

	/* read the first user */
	if(num_records > 0){
		read_user_better(0, current_user);
		sprintf(filename, "../data/sorted_user/0100000/user_%07d.dat", 0);
		start_tree(current_user->location_id, filename);
	}

	/* read the rest of the users */
	for(i = 1; i < num_records; i++) {
		read_user_better(i, current_user);
		sprintf(filename, "../data/sorted_user/0100000/user_%07d.dat", i);
		insert(current_user->location_id, filename);
	}

	free(current_user);
	return 0;
}

void insert(int key, char* value) {

	char leaf_filename[TEXT_SHORT];
	
	/* Every time we malloc a user_node, before we read into it
		we call fill_user_node to avoid having garbage */
	user_node* leaf_user_node = (user_node*) malloc(sizeof(user_node));
	fill_user_node(leaf_user_node);

	/* find leaf we want to insert into */
	get_leaf_for_insert(key, leaf_filename);
	read_user_node(leaf_user_node, leaf_filename);

		/* If the leaf isn't full, insert and be done */
	if(leaf_user_node->num_filled < NUM_KEYS){
		leaf_insert(leaf_filename, key, value);
	} else {
		/* Otherwise split and insert */
		leaf_insert_and_split(leaf_filename, key, value);
	}

	free(leaf_user_node);
}

void start_tree(int key, char* value){

	user_node* root = (user_node*) malloc(sizeof(user_node));
	fill_user_node(root);

	/* Set the initial values of the root with one child */
	strncpy(root->filename, USER_ROOT_PATH, TEXT_SHORT);
	strncpy(root->children[0], value, TEXT_SHORT);
	root->is_leaf = TRUE;
	root->keys[0] = key;
	root->num_filled = 1;
	write_user_node(root);

	free(root);
}

/* Finds the leaf that the key will be inserted into */
void get_leaf_for_insert(int key, char* leaf){

	int i, start, end, mid;
	user_node* current_user_node = (user_node*) malloc(sizeof(user_node));
	fill_user_node(current_user_node);
	read_user_node(current_user_node, (char*)&USER_ROOT_PATH);

	while(current_user_node->is_leaf == FALSE){

		/* i becomes the index of the child to look in next */
		i = 0;
		while (i < current_user_node->num_filled && key >= current_user_node->keys[i]){
			i++;
		}
		
		/* binary search array of "length" num_filled */
		start = 0;
		end = current_user_node->num_filled;
		int found = 0;
		while(end >= start && found ==FALSE){
			middle = (start + end)/ 2;
printf("Comparing %d with %d\n", current_user_node->keys[middle], key );
			
			if(middle - 1 < 0 && end == 0){
			printf("first if\n");
				middle = 0;
				found = TRUE;
			}else if(start == current_user_node->num_filled){
				printf("2 if\n");
				middle = start;
				found = TRUE;
			}else if(current_user_node->keys[middle] >= key 
				&& current_user_node->keys[middle-1] < key){
				printf("3 if\n");
				found = TRUE;
			}else if (current_user_node->keys[middle] < key){
				printf("4 if\n");
				start = middle + 1;
			}else if (current_user_node->keys[middle] >= key){
				printf("5 if\n");
				end = middle - 1;
			}
		}
		
		if(i != middle) {
			printf("i: %d  middle: %d !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n", i, middle);
		}else{
			printf("It worked\n");
		}
		
		i = start;
		
		
		
		
		read_user_node(current_user_node, current_user_node->children[i]);
	}
	
	/* saves the name of the leaf user_node that it belongs in */
	strncpy(leaf, current_user_node->filename, TEXT_SHORT);
	free(current_user_node);
}

void leaf_insert(char* leaf_filename, int key, char* value){

	user_node* leaf_user_node = (user_node *) malloc(sizeof(user_node));
	fill_user_node(leaf_user_node);
	read_user_node(leaf_user_node, leaf_filename);

	int i, j, start, end, mid;

	/* Get the index of the spot where key should be inserted */
	j = 0;
	while(j < leaf_user_node->num_filled && leaf_user_node->keys[j] <= key){
		j++;
	}
	
/* binary search array of "length" num_filled */
	start = 0;
	end = leaf_user_node->num_filled;
	int found = 0;
	while(end >= start && found ==FALSE){
		middle = (start + end)/ 2;
printf("Comparing %d with %d\n", leaf_user_node->keys[middle], key );
		if(middle - 1 < 0 && end == 0){
		printf("first if\n");
			middle = 0;
			found = TRUE;
		}else if(start == leaf_user_node->num_filled){
			printf("2 if\n");
			middle = start;
			found = TRUE;
		}else if(leaf_user_node->keys[middle] > key 
			&& leaf_user_node->keys[middle-1] <= key){
			printf("3 if\n");
			found = TRUE;
		}else if (leaf_user_node->keys[middle] <= key){
			printf("4 if\n");
			start = middle + 1;
		}else if (leaf_user_node->keys[middle] > key){
			printf("5 if\n");
			end = middle - 1;
		}
	}

	if(j != middle) {
		printf("j: %d  middle: %d !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n", j, middle);
	}else{
		printf("It worked\n");
	}
	
	j = start;
	

	/* Shift to make space for new key */
	for(i = leaf_user_node->num_filled - 1; i >= j; i--){
		leaf_user_node->keys[i+1] = leaf_user_node->keys[i];
		strncpy(leaf_user_node->children[i+1], leaf_user_node->children[i], TEXT_SHORT);
	}

	/* Fill in new key */
	leaf_user_node->keys[j] = key;
	strncpy(leaf_user_node->children[j], value, TEXT_SHORT);
	leaf_user_node->num_filled++;
	write_user_node(leaf_user_node);

	free(leaf_user_node);
}

void leaf_insert_and_split(char* leaf_filename, int key, char* value){

	/* the user_node we are inserting into */
	user_node* leaf_user_node = (user_node *) malloc(sizeof(user_node));
	fill_user_node(leaf_user_node);
	read_user_node(leaf_user_node, leaf_filename);
	
	int i, j, middle, insert_index, key_for_parent;
	char filename[TEXT_SHORT];

	char filename_for_parent[TEXT_SHORT];

	int copied_keys[FAN_OUT];
	char copied_children[FAN_OUT][TEXT_SHORT];

	/* new user_node to be the sibling */
	user_node* new_leaf = (user_node*) malloc(sizeof(user_node));
	fill_user_node(new_leaf);
	sprintf(filename, "../data/user_tree/user_node_%06li.dat", user_node_count);
	strncpy(new_leaf->filename, filename, TEXT_SHORT);
	user_node_count++;
	strncpy(new_leaf->parent, "\0", TEXT_SHORT);
	new_leaf->is_leaf = TRUE;
	new_leaf->num_filled = 0;
	
	middle = (NUM_KEYS / 2) + 1;
	
	/* Get the index of the spot where key should be inserted */
	insert_index = 0;
	while(insert_index < leaf_user_node->num_filled && leaf_user_node->keys[insert_index] < key){
		insert_index++;
	}
	
	/* copy the keys into an array that is one bigger, so it can be split */
	for (i = 0, j = 0; i < leaf_user_node->num_filled; i++, j++)
	{
		if(j == insert_index){
			j++;
		}
		copied_keys[j] = leaf_user_node->keys[i];
		strncpy(copied_children[j], leaf_user_node->children[i], TEXT_SHORT);
	}
	copied_keys[insert_index] = key;
	strncpy(copied_children[insert_index], value, TEXT_SHORT);

	/* Reset num filled so we can count back up as we add
		the children and keys after the split */
	leaf_user_node->num_filled = 0;
	for (i = 0; i < middle; i++)
	{
		leaf_user_node->keys[i] = copied_keys[i];
		strncpy(leaf_user_node->children[i], copied_children[i], TEXT_SHORT);
		leaf_user_node->num_filled++;
	}
	for (j = 0, i = middle; i < FAN_OUT; j++, i++)
	{
		new_leaf->keys[j] = copied_keys[i];
		strncpy(new_leaf->children[j], copied_children[i], TEXT_SHORT);
		new_leaf->num_filled++;
	}
	
	/* the sibling of the new leaf is the sibling of the leaf before the split */
	strncpy(new_leaf->children[FAN_OUT - 1], leaf_user_node->children[FAN_OUT - 1], TEXT_SHORT);
	
	/* sibling of the old leaf is the new leaf itself */
	strncpy(leaf_user_node->children[FAN_OUT - 1], new_leaf->filename, TEXT_SHORT);
	
	/* the siblings have the same parent */
	strncpy(new_leaf->parent, leaf_user_node->parent, TEXT_SHORT);
	
	/* name and child for the parent */
	strncpy(filename_for_parent, leaf_user_node->filename, TEXT_SHORT);
	key_for_parent = new_leaf->keys[0];

	/* save before insert_key_into_parent because it depends on these user_nodes */
	write_user_node(new_leaf);	
	write_user_node(leaf_user_node);

	/* copy the key up into the parent */
	insert_key_into_parent(key_for_parent, leaf_user_node->filename, new_leaf->filename);
	free(leaf_user_node);
	free(new_leaf);
}



void insert_key_into_parent(int key, char* old_child, char* new_child) {

	char filename[TEXT_SHORT];
	int i, j;

	user_node* old_child_user_node = (user_node *) malloc(sizeof(user_node));
	fill_user_node(old_child_user_node);
	read_user_node(old_child_user_node, old_child);
	user_node* new_child_user_node = (user_node *) malloc(sizeof(user_node));
	fill_user_node(new_child_user_node);
	read_user_node(new_child_user_node, new_child);

	if(strcmp(old_child_user_node->parent, "\0") == 0){
		/* Make a new root user_node, and insert key there */
		user_node* root = (user_node *) malloc(sizeof(user_node));
		fill_user_node(root);
		strncpy(root->filename, USER_ROOT_PATH, TEXT_SHORT);
		root->num_filled = 1;
		strncpy(root->parent, "\0", TEXT_SHORT);
		root->is_leaf = FALSE;
		root->keys[0] = key;

		sprintf(filename, "../data/user_tree/user_node_%06li.dat", user_node_count);
		strncpy(old_child_user_node->filename, filename, TEXT_SHORT);
		user_node_count++;

		strncpy(root->children[0], old_child_user_node->filename, TEXT_SHORT);
		strncpy(root->children[1], new_child_user_node->filename, TEXT_SHORT);

		strncpy(old_child_user_node->parent, USER_ROOT_PATH, TEXT_SHORT);
		strncpy(new_child_user_node->parent, USER_ROOT_PATH, TEXT_SHORT);

		write_user_node(root);
		write_user_node(old_child_user_node);
		write_user_node(new_child_user_node);

		user_node* child_iter = (user_node *) malloc(sizeof(user_node));
		fill_user_node(child_iter);

		if(old_child_user_node->is_leaf == FALSE){
			for(i = 0; i <= old_child_user_node->num_filled; i++) {
					read_user_node(child_iter, old_child_user_node->children[i]);
					strncpy(child_iter->parent, old_child_user_node->filename, TEXT_SHORT);
			}
		}

		free(child_iter);
		free(root);
	}else{
		/* Modify existing parent user_node */

		user_node* parent_user_node = (user_node*) malloc(sizeof(user_node));
		fill_user_node(parent_user_node);
		read_user_node(parent_user_node, old_child_user_node->parent);

		/* Find which child of the parent corresponds to 'old child user_node' */
		i = 0;
		while(i <= parent_user_node->num_filled && strcmp(parent_user_node->children[i], old_child) !=0){
			i++;
		}
		
		/* If parent is not full */
		if(parent_user_node->num_filled < NUM_KEYS){ 
			/* Go backwards through the children of the parent, shifting children over */
			for (j = parent_user_node->num_filled; j > i; j--)
			{
				parent_user_node->keys[j] = parent_user_node->keys[j-1];
				strncpy(parent_user_node->children[j+1], parent_user_node->children[j], TEXT_SHORT);
			}

			parent_user_node->keys[i] = key;
			strncpy(parent_user_node->children[i+1], new_child, TEXT_SHORT);
			parent_user_node->num_filled++;
			write_user_node(parent_user_node);
		} else {
			user_node_insert_and_split(parent_user_node->filename, new_child, key, i);
		}

		free(parent_user_node);
	}

	free(old_child_user_node);
	free(new_child_user_node);

}

void user_node_insert_and_split(char* parent, char* new_child, int key, int index){

	user_node* parent_user_node = (user_node *) malloc(sizeof(user_node));
	fill_user_node(parent_user_node);
	read_user_node(parent_user_node, parent);
	
	user_node* new_child_user_node = (user_node *) malloc(sizeof(user_node));
	fill_user_node(new_child_user_node);
	read_user_node(new_child_user_node, new_child);
	
	user_node* child_iter = (user_node *) malloc(sizeof(user_node));
	fill_user_node(child_iter);

	char filename[TEXT_SHORT];
	int copied_keys[FAN_OUT];
	char copied_children[FAN_OUT+1][TEXT_SHORT];
	int i, j, middle;
	int key_for_parent;
	
	/* copy the children and keys into an array that is one bigger
		so we can split them */	
	for(i = 0, j = 0; i < NUM_KEYS; i++, j++) {
		if(j == index) {
			/* skip the one where we are inserting the key */
			j++;
		}
		copied_keys[j] = parent_user_node->keys[i];
	}
	for(i = 0, j = 0; i < NUM_KEYS + 1; i++, j++) {
		if(j == index + 1) {
			/* skip the one where we are inserting the key */
			j++;
		} 
		strncpy(copied_children[j], parent_user_node->children[i], TEXT_SHORT);
	}

	copied_keys[index] = key;
	strncpy(copied_children[index + 1], new_child, TEXT_SHORT);

	middle = (FAN_OUT + 1) / 2;

	/* make the new sibling */
	user_node* new_user_node = (user_node*) malloc(sizeof(user_node));
	fill_user_node(new_user_node);
	sprintf(filename, "../data/user_tree/user_node_%06li.dat", user_node_count);
	strncpy(new_user_node->filename, filename, TEXT_SHORT);
	user_node_count++;
	strncpy(new_user_node->parent, "\0", TEXT_SHORT);
	new_user_node->is_leaf = FALSE;
	new_user_node->num_filled = 0;

	/* parent user_node is now the left sibling*/
	parent_user_node->num_filled = 0;
	for(i = 0; i < middle - 1; i++) {
		strncpy(parent_user_node->children[i], copied_children[i], TEXT_SHORT);
		parent_user_node->keys[i] = copied_keys[i];
		parent_user_node->num_filled++;
	}
	/* save the middle key to copy to parent,
		don't copy it because it's a leaf user_node */
	strncpy(parent_user_node->children[i], copied_children[i], TEXT_SHORT);
	key_for_parent = copied_keys[i];
	
	/* fill the right sibling */
	for(i = middle, j = 0; i < FAN_OUT; i++, j++) {
		strncpy(new_user_node->children[j], copied_children[i], TEXT_SHORT);
		new_user_node->keys[j] = copied_keys[i];
		new_user_node->num_filled++;
	}
	strncpy(new_user_node->children[j], copied_children[i], TEXT_SHORT);
	strncpy(new_user_node->parent, parent_user_node->parent, TEXT_SHORT);
	
	write_user_node(parent_user_node);
	write_user_node(new_child_user_node);
	write_user_node(new_user_node);

	/* saves the new name of the parent in its children */
	for(i = 0; i <= new_user_node->num_filled; i++) {
		read_user_node(child_iter, new_user_node->children[i]);
		strncpy(child_iter->parent, new_user_node->filename, TEXT_SHORT);
		write_user_node(child_iter);
	}

	insert_key_into_parent(key_for_parent, parent_user_node->filename, new_user_node->filename);

	free(child_iter);
	free(new_user_node);
	free(parent_user_node);
	free(new_child_user_node);
}

/* filling the user_node to avoid having garbage data */
void fill_user_node(user_node* empty_user_node) {
	int i;
	strncpy(empty_user_node->filename, "\0", TEXT_SHORT);

	for(i = 0; i < FAN_OUT; i++) {
		strncpy(empty_user_node->children[i], "\0", TEXT_SHORT);
	}	

	strncpy(empty_user_node->parent, "\0", TEXT_SHORT);
	
	for(i = 0; i < NUM_KEYS; i++) {
		empty_user_node->keys[i] = -1;
	}

	empty_user_node->is_leaf = FALSE;
	empty_user_node->num_filled = 0;
}
