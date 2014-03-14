#include "structures.h"
#include "record.h"
#include "bp_structs.c"

void read_user(char *filename, user* user_read) {
	FILE *fp = NULL;
	fp = fopen(filename, "rb");
	fread(&(user_read->user_id), sizeof(long), 1, fp);
	fread(&(user_read->user_name[0]), sizeof(char), TEXT_SHORT, fp);
	fread(&(user_read->location_id), sizeof(long), 1, fp);
	fclose(fp);
}
void read_location(FILE *fp, location* location_read){
	fread(&(location_read->location_id), sizeof(long), 1, fp);
	fread(&(location_read->city[0]), sizeof(char), TEXT_SHORT, fp);
	fread(&(location_read->state[0]), sizeof(char), TEXT_SHORT, fp);
}
void read_database_message(FILE *fp, message* message_read) {
	fread(&(message_read->message_id), sizeof(long), 1, fp);
	fread(&(message_read->user_id), sizeof(long), 1, fp);
	fread(&(message_read->date_id), sizeof(long), 1, fp);
	fread(&(message_read->time_id), sizeof(long), 1, fp);
	
	fread(&(message_read->text[0]), sizeof(char), TEXT_LONG, fp);
}
void read_date(FILE *fp, date* date_read) {
	fread(&(date_read->date_id), sizeof(long), 1, fp);
	fread(&(date_read->year), sizeof(int), 1, fp);
	fread(&(date_read->month), sizeof(int), 1, fp);
	fread(&(date_read->day), sizeof(int), 1, fp);
}
void read_time(FILE *fp, time* time_read) {
	fread(&(time_read->time_id), sizeof(long), 1, fp);
	fread(&(time_read->hour), sizeof(int), 1, fp);
	fread(&(time_read->minute), sizeof(int), 1, fp);
}

void write_user(FILE *fp, user* user_write) {
	fwrite(&(user_write->user_id),sizeof(long), 1, fp);
	fwrite((user_write->user_name),sizeof(char), TEXT_SHORT, fp);
	fwrite(&(user_write->location_id),sizeof(long), 1, fp);
}
void write_location(FILE *fp, location* location_write) {       	
	fwrite(&(location_write->location_id),sizeof(long),1, fp);
	fwrite((location_write->city),sizeof(char), TEXT_SHORT, fp);
	fwrite((location_write->state),sizeof(char), TEXT_SHORT, fp);
}
void write_message(FILE *fp, message* message_write) {
	fwrite(&(message_write->message_id), sizeof(long), 1, fp);
	fwrite(&(message_write->user_id), sizeof(long), 1, fp);
	fwrite(&(message_write->date_id), sizeof(long), 1, fp);
	fwrite(&(message_write->time_id), sizeof(long), 1, fp);
	fwrite((message_write->text), sizeof(char), TEXT_LONG, fp);
}
void write_date(FILE *fp, date* date_write) {
	fwrite(&(date_write),sizeof(long), 1, fp);
	fwrite(&(date_write),sizeof(int), 1, fp);
	fwrite(&(date_write),sizeof(int), 1, fp);
	fwrite(&(date_write),sizeof(int), 1, fp);	 
}
void write_time(FILE *fp, time* time_write) {
	fwrite(&(time_write->time_id), sizeof(long), 1, fp);	    	
	fwrite(&(time_write->hour), sizeof(int), 1, fp);
	fwrite(&(time_write->minute), sizeof(int), 1, fp);
}




void read_node(node* to_read, char* filename) {
printf("reading node: %s\n", filename);
	int i;
	FILE *fp = NULL;
	fp = fopen(filename, "rb");

	strncpy(to_read->filename, filename, TEXT_SHORT);
	

	for(i = 0; i < FAN_OUT; i++) {
		fread(&(to_read->children[i]), sizeof(char), TEXT_SHORT, fp);
	}

	for(i = 0; i < FAN_OUT - 1; i++) {
		fread(&(to_read->keys[i]), sizeof(long), 1, fp);
	}

	fread(&(to_read->is_leaf), sizeof(int), 1, fp);
	fread(&(to_read->num_filled), sizeof(int), 1, fp);
	fread((to_read->parent), sizeof(char), TEXT_SHORT, fp);

	fclose(fp);	
}

void write_node(node* to_save) {
	int i;
	FILE *fp = NULL;
	fp = fopen(to_save->filename, "w");
	for(i = 0; i < FAN_OUT; i++) {
		/* verify that this is not awful */
		fwrite(&(to_save->children[i]), sizeof(char), TEXT_SHORT, fp);
	}
	for(i = 0; i < FAN_OUT - 1; i++) {
		fwrite(&(to_save->keys[i]), sizeof(long), 1, fp);
	}
	fwrite(&(to_save->is_leaf), sizeof(int), 1, fp);
	fwrite(&(to_save->num_filled), sizeof(int), 1, fp);
	fwrite((to_save->parent), sizeof(char), TEXT_SHORT, fp);
	fclose(fp);
}

void print_node(node* to_print) {
	int i;

	printf("Node: %s\n", to_print->filename);

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

void print_tree(char* filename) {
	
	int i;

	node* current_node = (node*) malloc(sizeof(node));
	read_node(current_node, filename);
	print_node(current_node);

	if(current_node->is_leaf == FALSE) {
		for(i = 0; i < current_node->num_filled; i++) {
			print_tree(current_node->children[i]);
		}
	}

	free(current_node);
}
