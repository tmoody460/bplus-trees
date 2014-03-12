#include "bp_structs.c"
#include "structures.h"
#include <string.h>
#include "rw_methods.c"

#define ROOT_PATH "user_tree/root.dat"

void insert(long key, char* value);
void create_tree(long key, char* value);
void get_leaf(long key, char* leaf);
void insert_without_split(char* leaf_filename, long key, char* value);
void insert_with_split(char* leaf_filename, long key, char* value);
void insert_key_into_parent(long key, char* old_child, char* new_child);
void insert_node_with_split(char* old_child, char* new_child, long key, int index);
void fill_node(node* empty_node);


long node_count = 0;

int main(int argc, char** argv) {
	int i;
	char filename[TEXT_SHORT];
	if (argc != 2) {
        fprintf(stderr, "Usage: %s #users\n", argv[0]);
        exit(0);
    }

	int num_records = atoi(argv[1]);

	user* current_user;
	

	current_user = (user *) malloc(sizeof(user));

	if(num_records > 0){
		sprintf(filename, "users/user_%06d.dat", 0);
		read_user(filename, current_user);
		create_tree(current_user->location_id, filename);
	}

	for(i = 1; i < num_records; i++) {
printf("Loop (i: %d)\n", i);
		sprintf(filename, "users/user_%06d.dat", i);
printf("sprinted\n");
		read_user(filename, current_user);
printf("read user\n");
		insert(current_user->location_id, filename);
printf("inserted key: %li\n", current_user->location_id);
		//print_tree(ROOT_PATH);
		printf("Inserted: %s\n", filename);
	}

	node* root = (node*) malloc(sizeof(node));
	fill_node(root);
	read_node(root, ROOT_PATH);
	print_tree(root->filename);
	free(root);
	
	free(current_user);
	

	return 0;
}

void insert(long key, char* value) {
	printf("Insert (Key: %li, Value: %s)\n", key, value);

	//duplication check?

	char leaf_filename[TEXT_SHORT];
	node* leaf_node = (node*) malloc(sizeof(node));
	fill_node(leaf_node);

	// find leaf we want to mess with
	get_leaf(key, leaf_filename);
	read_node(leaf_node, leaf_filename);
printf("read the next node\n");
	// If leaf isn't full insert and be done, retunr
	if(leaf_node->num_filled < NUM_KEYS){
		insert_without_split(leaf_filename, key, value);
	} else {
	// Otherwise split and insert and be done, return

		insert_with_split(leaf_filename, key, value);
	}

	free(leaf_node);
}

void create_tree(long key, char* value){
printf("Create Tree modified (Key: %li, Value: %s)\n", key, value);
	node* root = (node*) malloc(sizeof(node));
	fill_node(root);

	strncpy(root->filename, ROOT_PATH, TEXT_SHORT);
	strncpy(root->children[0], value, TEXT_SHORT);
	strncpy(root->children[1], "\0", TEXT_SHORT);
	strncpy(root->children[2], "\0", TEXT_SHORT);
	root->keys[1] = -1;
	root->is_leaf = TRUE;
	root->keys[0] = key;
	strncpy(root->parent, "\0", TEXT_SHORT);
	root->num_filled = 1;
	write_node(root);
//print_node(root);
	free(root);
}

// Leaf is the destination of the found leaf filename
void get_leaf(long key, char* leaf){
	int i;
	node* current_node = (node*) malloc(sizeof(node));
	fill_node(current_node);
	read_node(current_node, ROOT_PATH);

	while(current_node->is_leaf == FALSE){

//TODO: BINARY SEARCH?
		i = 0;
		while (i < current_node->num_filled && key >= current_node->keys[i]){
			i++;
		}

		read_node(current_node, current_node->children[i]);
	}
	
	strncpy(leaf, current_node->filename, TEXT_SHORT);
	free(current_node);
}

void insert_without_split(char* leaf_filename, long key, char* value){
printf("Insert without split\n");
	node* leaf_node = (node *) malloc(sizeof(node));
	fill_node(leaf_node);
	read_node(leaf_node, leaf_filename);


	int i;
	int j = 0;

	// Get the index of the spot where key should be inserted
	while(j < leaf_node->num_filled && leaf_node->keys[j] < key){
		j++;
	}

	// Shift to make space for new key
	for(i = leaf_node->num_filled - 1; i >= j; i--){
		leaf_node->keys[i+1] = leaf_node->keys[i];
		strncpy(leaf_node->children[i+1], leaf_node->children[i], TEXT_SHORT);
	}

	// Fill in new key
	leaf_node->keys[j] = key;
	strncpy(leaf_node->children[j], value, TEXT_SHORT);
	leaf_node->num_filled++;
	write_node(leaf_node);
print_node(leaf_node);
	free(leaf_node);
}

void insert_with_split(char* leaf_filename, long key, char* value){
printf("Insert with split\n");
	node* leaf_node = (node *) malloc(sizeof(node));
	fill_node(leaf_node);
	read_node(leaf_node, leaf_filename);
	int i, j, middle, insert_index;
	char filename[TEXT_SHORT];

	long key_for_parent;
	char filename_for_parent[TEXT_SHORT];

	long copied_keys[FAN_OUT];
	char copied_children[FAN_OUT][TEXT_SHORT];

	node* new_leaf = (node*) malloc(sizeof(node));
	fill_node(new_leaf);

	sprintf(filename, "user_tree/node_%06li.dat", node_count);
	strncpy(new_leaf->filename, filename, TEXT_SHORT);
	node_count++;
	strncpy(new_leaf->parent, "\0", TEXT_SHORT);
	new_leaf->is_leaf = TRUE;
	new_leaf->num_filled = 0;
	middle = (NUM_KEYS + 1) / 2;
	// Get the index of the spot where key should be inserted
	insert_index = 0;
	while(insert_index < leaf_node->num_filled && leaf_node->keys[insert_index] < key){
		insert_index++;
	}
	for (i = 0, j = 0; i < leaf_node->num_filled; i++, j++)
	{
		if(j == insert_index){
			j++;
		}
		copied_keys[j] = leaf_node->keys[i];
		strncpy(copied_children[j], leaf_node->children[i], TEXT_SHORT);
	}
	copied_keys[insert_index] = key;
	strncpy(copied_children[insert_index], value, TEXT_SHORT);

	// Reset num filled so we can count back up as we add them
	leaf_node->num_filled = 0;
	for (i = 0; i < middle; i++)
	{
		leaf_node->keys[i] = copied_keys[i];
		strncpy(leaf_node->children[i], copied_children[i], TEXT_SHORT);
		leaf_node->num_filled++;
	}
	for (j = 0, i = middle; i < FAN_OUT; j++, i++)
	{
		new_leaf->keys[j] = copied_keys[i];
		strncpy(new_leaf->children[j], copied_children[i], TEXT_SHORT);
		new_leaf->num_filled++;
	}
	strncpy(new_leaf->children[FAN_OUT - 1], leaf_node->children[FAN_OUT - 1], TEXT_SHORT);
	strncpy(leaf_node->children[FAN_OUT - 1], new_leaf->filename, TEXT_SHORT);
	strncpy(new_leaf->parent, leaf_node->parent, TEXT_SHORT);
	strncpy(filename_for_parent, leaf_node->filename, TEXT_SHORT);
	key_for_parent = new_leaf->keys[0];

	write_node(new_leaf);	
	write_node(leaf_node);
print_node(leaf_node);
print_node(new_leaf);

	insert_key_into_parent(key_for_parent, leaf_node->filename, new_leaf->filename);
	free(leaf_node);
	free(new_leaf);
}



void insert_key_into_parent(long key, char* old_child, char* new_child) {
printf("Insert key into parent\n");
	char filename[TEXT_SHORT];

	node* old_child_node = (node *) malloc(sizeof(node));
	fill_node(old_child_node);
	read_node(old_child_node, old_child);
	node* new_child_node = (node *) malloc(sizeof(node));
	fill_node(new_child_node);
	read_node(new_child_node, new_child);

	if(strcmp(old_child_node->parent, "\0") == 0){
		// Make a new root, and insert key there

		node* root = (node *) malloc(sizeof(node));
		fill_node(root);
		strncpy(root->filename, ROOT_PATH, TEXT_SHORT);
		root->num_filled = 1;
		strncpy(root->parent, "\0", TEXT_SHORT);
		root->is_leaf = FALSE;
		root->keys[0] = key;

		sprintf(filename, "user_tree/node_%06li.dat", node_count);
		strncpy(old_child_node->filename, filename, TEXT_SHORT);
		node_count++;

		strncpy(root->children[0], old_child_node->filename, TEXT_SHORT);
		strncpy(root->children[1], new_child_node->filename, TEXT_SHORT);

		strncpy(old_child_node->parent, ROOT_PATH, TEXT_SHORT);
		strncpy(new_child_node->parent, ROOT_PATH, TEXT_SHORT);

		write_node(root);
		write_node(old_child_node);
		write_node(new_child_node);

		node* child_iter = (node *) malloc(sizeof(node));
		fill_node(child_iter);

		if(old_child_node->is_leaf == FALSE){
			int i;
			for(i = 0; i <= old_child_node->num_filled; i++) {
					read_node(child_iter, old_child_node->children[i]);
					strncpy(child_iter->parent, old_child_node->filename, TEXT_SHORT);
			printf("[] Assigning %s as parent of %s\n", old_child_node->filename, child_iter->filename);
					write_node(child_iter);
			//print_node(child_iter);
			}
		}

		free(child_iter);
print_node(root);
print_node(old_child_node);
print_node(new_child_node);
		
		free(root);
	}else{
		// Modify existing parent node

		node* parent_node = (node*) malloc(sizeof(node));
		fill_node(parent_node);
		read_node(parent_node, old_child_node->parent);

		// Find which child of the parent corresponds to 'old child node'
		int i = 0;
		while(i <= parent_node->num_filled && strcmp(parent_node->children[i], old_child) !=0){
			i++;
		}
		// If parent is not full
		if(parent_node->num_filled < NUM_KEYS){ 

			int j;

			// Go backwards through the children of the parent, shifting children over
			for (j = parent_node->num_filled; j > i; j++)
			{
				parent_node->keys[j] = parent_node->keys[j-1];
				strncpy(parent_node->children[j+1], parent_node->children[j], TEXT_SHORT);
			}

			parent_node->keys[i] = key;
			strncpy(parent_node->children[i+1], new_child, TEXT_SHORT);
			parent_node->num_filled++;
			write_node(parent_node);
print_node(parent_node);
		}else{
			
			insert_node_with_split(parent_node->filename, new_child, key, i);
		}

		free(parent_node);
	}

	free(old_child_node);
	free(new_child_node);

}

void insert_node_with_split(char* parent, char* new_child, long key, int index){
printf("With Split, node\n");
	node* parent_node = (node *) malloc(sizeof(node));
	fill_node(parent_node);
	read_node(parent_node, parent);
	node* new_child_node = (node *) malloc(sizeof(node));
	fill_node(new_child_node);
	read_node(new_child_node, new_child);
	
	node* child_iter = (node *) malloc(sizeof(node));
	fill_node(child_iter);

	char filename[TEXT_SHORT];
	long copied_keys[FAN_OUT];
	char copied_children[FAN_OUT+1][TEXT_SHORT];
	int i, j, middle;
	long key_for_parent;
	
print_node(parent_node);
	
	for(i = 0, j = 0; i < NUM_KEYS; i++, j++) {
		if(j == index) {
			//skip the one where we are inserting the key
			j++;
		}
		copied_keys[j] = parent_node->keys[i];
		printf("%li\n", copied_keys[j]);

	}
	for(i = 0, j = 0; i < NUM_KEYS + 1; i++, j++) {
		if(j == index + 1) {
			//skip the one where we are inserting the key
			j++;
		} 
		strncpy(copied_children[j], parent_node->children[i], TEXT_SHORT);
	}

	copied_keys[index] = key;
	printf("%li\n", copied_keys[index]);
	strncpy(copied_children[index + 1], new_child, TEXT_SHORT);

	middle = (FAN_OUT + 1) / 2;

	node* new_node = (node*) malloc(sizeof(node));
	fill_node(new_node);
	sprintf(filename, "user_tree/node_%06li.dat", node_count);
	strncpy(new_node->filename, filename, TEXT_SHORT);
	node_count++;
	strncpy(new_node->parent, "\0", TEXT_SHORT);
	new_node->is_leaf = FALSE;
	new_node->num_filled = 0;
//TODO: check if we need to rewrite stuff here, or just update num_filled
	parent_node->num_filled = 0;
	for(i = 0; i < middle - 1; i++) {
		strncpy(parent_node->children[i], copied_children[i], TEXT_SHORT);
		parent_node->keys[i] = copied_keys[i];
		parent_node->num_filled++;
	}
	strncpy(parent_node->children[i], copied_children[i], TEXT_SHORT);
	key_for_parent = copied_keys[i];
//^ save the middle key to copy to parent
	for(i = middle, j = 0; i < FAN_OUT; i++, j++) {
		strncpy(new_node->children[j], copied_children[i], TEXT_SHORT);
		new_node->keys[j] = copied_keys[i];
		new_node->num_filled++;
	}
	strncpy(new_node->children[j], copied_children[i], TEXT_SHORT);
	strncpy(new_node->parent, parent_node->parent, TEXT_SHORT);
	//saves the name of the parent in all the children
	write_node(parent_node);
	write_node(new_child_node);
	write_node(new_node);

	for(i = 0; i <= new_node->num_filled; i++) {
		read_node(child_iter, new_node->children[i]);
		strncpy(child_iter->parent, new_node->filename, TEXT_SHORT);
printf("Assigning %s as parent of %s\n", new_node->filename, child_iter->filename);
		write_node(child_iter);
//print_node(child_iter);
	}

print_node(parent_node);
print_node(new_node);
print_node(new_child_node);

	insert_key_into_parent(key_for_parent, parent_node->filename, new_node->filename);

	free(child_iter);
	free(new_node);
	free(parent_node);
	free(new_child_node);
}


void fill_node(node* empty_node) {
	int i;
	strncpy(empty_node->filename, "\0", TEXT_SHORT);

	for(i = 0; i < FAN_OUT; i++) {
		strncpy(empty_node->children[i], "\0", TEXT_SHORT);
	}	

	strncpy(empty_node->parent, "\0", TEXT_SHORT);
	
	for(i = 0; i < NUM_KEYS; i++) {
		empty_node->keys[i] = -1;
	}

	empty_node->is_leaf = FALSE;
	empty_node->num_filled = 0;
}
