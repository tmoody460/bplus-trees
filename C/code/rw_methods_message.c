#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bp_structs.c"

void read_message_node(message_node* to_read, char* filename) {

	int i;
	FILE *fp = NULL;
	fp = fopen(filename, "rb");

	strncpy(to_read->filename, filename, TEXT_SHORT);
	

	for(i = 0; i < FAN_OUT; i++) {
		fread(&(to_read->children[i]), sizeof(char), TEXT_SHORT, fp);
	}

	for(i = 0; i < FAN_OUT - 1; i++) {
		fread(&(to_read->keys[i]), sizeof(int), 1, fp);
	}

	fread(&(to_read->is_leaf), sizeof(int), 1, fp);
	fread(&(to_read->num_filled), sizeof(int), 1, fp);
	fread((to_read->parent), sizeof(char), TEXT_SHORT, fp);

	fclose(fp);	
}

void write_message_node(message_node* to_save) {
	int i;
	FILE *fp = NULL;
	fp = fopen(to_save->filename, "w");
	for(i = 0; i < FAN_OUT; i++) {
		/* verify that this is not awful */
		fwrite(&(to_save->children[i]), sizeof(char), TEXT_SHORT, fp);
	}
	for(i = 0; i < FAN_OUT - 1; i++) {
		fwrite(&(to_save->keys[i]), sizeof(int), 1, fp);
	}
	fwrite(&(to_save->is_leaf), sizeof(int), 1, fp);
	fwrite(&(to_save->num_filled), sizeof(int), 1, fp);
	fwrite((to_save->parent), sizeof(char), TEXT_SHORT, fp);
	fclose(fp);
}

void print_message_node(message_node* to_print) {
	int i;

	printf("message_node: %s\n", to_print->filename);

	for(i = 0; i < to_print->num_filled; i++) {
		printf("\t%s\n", to_print->children[i]);
	}

	for(i = 0; i < to_print->num_filled; i++) {
		if(i < FAN_OUT - 1) {
			printf("\t%li\n", to_print->keys[i]);
		}
	}

	if(to_print->is_leaf == 0) {
		printf("\tIs leaf: %d (no it's not)\n", to_print->is_leaf);
	} else {
		printf("\tIs leaf: %d (yes it is)\n", to_print->is_leaf);
	}

	printf("\tNum filled: %d\n", to_print->num_filled);

	printf("\tParent: %s\n", to_print->parent);
	
	printf("\tSibling: %s\n", to_print->children[FAN_OUT - 1]);

}

void print_tree_message(char* filename) {
	
	int i;

	message_node* current_message_node = (message_node*) malloc(sizeof(message_node));
	read_message_node(current_message_node, filename);
	print_message_node(current_message_node);

	if(current_message_node->is_leaf == FALSE) {
		for(i = 0; i < current_message_node->num_filled; i++) {
			print_tree_message(current_message_node->children[i]);
		}
	}

	free(current_message_node);
}
