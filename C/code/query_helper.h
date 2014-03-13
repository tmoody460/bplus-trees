#ifndef __QUERY_HELPER_H__
#define __QUERY_HELPER_H__

#include <stdio.h>
#include <stdlib.h>
#include "tables.h"


#define TEXT_SHORT      64
#define TEXT_LONG       1024

typedef struct user_query_entry{
	user_table_entry_t user;
	user_query_entry *next;
} user_query_entry;



typedef struct message_query_entry{
	message_table_entry_t message;
	message_query_entry *next;
} message_query_entry;



typedef struct location_query_entry{
	location_table_entry_t location;
	location_query_entry *next;
} location_query_entry;



typedef struct senddate_query_entry{
	senddate_table_entry_t senddate;
	senddate_query_entry*next;
} senddate_query_entry;


typedef struct sendtime_query_entry{
	sendtime_table_entry_t sendtime;
	sendtime_query_entry *next;
} sendtime_query_entry;

void free_users(user_query_entry* head);
void free_locations(location_query_entry* head);
void free_messages(message_query_entry* head);
void free_senddates(senddate_query_entry* head);
void free_sendtimes(sendtime_query_entry* head);

void read_user_better(int user_id, user_table_entry_t *user_read);
void read_location_better(int location_id, location_table_entry_t *location_read);
void read_message_better(int message_id, message_table_entry_t *message_read);
void read_senddate_better(int senddate_id, senddate_table_entry_t *senddate_read);
void read_sendtime_better(int sendtime_id, sendtime_table_entry_t *sendtime_read);



#endif
