#include "query_helper.h"

void free_users(user_query_entry* head) {
	
	user_query_entry *temp;
	user_query_entry *to_free;
	
	to_free = head;

	
	while(to_free != NULL) {
		temp = to_free;
		to_free = to_free->next;
		free(temp);
	}
}

void free_locations(location_query_entry* head) {
	
	location_query_entry *temp;
	location_query_entry *to_free;
	
	to_free = head;

	
	while(to_free != NULL) {
		temp = to_free;
		to_free = to_free->next;
		free(temp);
	}
}

void free_messages(message_query_entry* head) {
	
	message_query_entry *temp;
	message_query_entry *to_free;
	
	to_free = head;

	
	while(to_free != NULL) {
		temp = to_free;
		to_free = to_free->next;
		free(temp);
	}
}

void free_senddates(senddate_query_entry* head) {
	
	senddate_query_entry *temp;
	senddate_query_entry *to_free;
	
	to_free = head;

	
	while(to_free != NULL) {
		temp = to_free;
		to_free = to_free->next;
		free(temp);
	}
}

void free_sendtimes(sendtime_query_entry* head) {
	
	sendtime_query_entry *temp;
	sendtime_query_entry *to_free;
	
	to_free = head;

	
	while(to_free != NULL) {
		temp = to_free;
		to_free = to_free->next;
		free(temp);
	}
}

void read_user_better(int user_id, user_table_entry_t *user_read){
	char filename[TEXT_SHORT];
	FILE *fp;
        
    /* open the corresponding file */
	int subdir = (user_id / 100000 + 1) * 100000;
    sprintf(filename, "../data/sorted_user/%07d/user_%07d.dat", 
        subdir,
        user_id);
    fp = fopen(filename,"rb");
     
    if (!fp) {
        fprintf(stderr, "Cannot open %s\n", filename);
        exit(0);
    }
	read_user(fp, user_read);
	fclose(fp);
	
}

void read_location_better(int location_id, location_table_entry_t *location_read){
	char filename[TEXT_SHORT];
	FILE *fp;
    /* open the corresponding file */
	int subdir = (location_id / 100000 + 1) * 100000;
    sprintf(filename, "../data/unique_location/%07d/location_%07d.dat", 
        subdir,
        location_id);
    fp = fopen(filename,"rb");
     
    if (!fp) {
        fprintf(stderr, "Cannot open %s\n", filename);
        exit(0);
    }
	read_location(fp, location_read);
	fclose(fp);
}

void read_message_better(int message_id, message_table_entry_t *message_read){
	char filename[TEXT_SHORT];
	FILE *fp;
	int subdir = (message_id / 100000 + 1) * 100000;
    sprintf(filename, "../data/sorted_message/%07d/message_%07d.dat", 
        subdir,
        message_id);
    fp = fopen(filename,"rb");
     
    if (!fp) {
        fprintf(stderr, "Cannot open %s\n", filename);
        exit(0);
    }
	read_message(fp, message_read);
	fclose(fp);
}

void read_senddate_better(int senddate_id, senddate_table_entry_t *senddate_read){;
	char filename[TEXT_SHORT];
	FILE *fp;
	int subdir = (senddate_id / 100000 + 1) * 100000;
    sprintf(filename, "../data/unique_senddate/%07d/senddate_%07d.dat", 
        subdir,
        senddate_id);
    fp = fopen(filename,"rb");
     
    if (!fp) {
        fprintf(stderr, "Cannot open %s\n", filename);
        exit(0);
    }
	read_senddate(fp, senddate_read);
	fclose(fp);
}

void read_sendtime_better(int sendtime_id, sendtime_table_entry_t *sendtime_read){
	char filename[TEXT_SHORT];
	FILE *fp;
	int subdir = (sendtime_id / 100000 + 1) * 100000;
    sprintf(filename, "../data/unique_sendtime/%07d/sendtime_%07d.dat", 
        subdir,
        sendtime_id);
    fp = fopen(filename,"rb");
     
    if (!fp) {
        fprintf(stderr, "Cannot open %s\n", filename);
        exit(0);
    }
	read_sendtime(fp, sendtime_read);
	fclose(fp);
}
