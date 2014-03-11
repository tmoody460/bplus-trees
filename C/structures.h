#ifndef __STRUCTURES_H__
#define __STRUCTURES_H__

#include "record.h"

typedef struct user {

	long user_id;
	char user_name[TEXT_SHORT];
	long location_id;
	struct user* next;

} user;


typedef struct message {

	long message_id;
	long user_id;
	long date_id;
	long time_id;
	char text[TEXT_LONG];
	struct message* next;

} message;

typedef struct date {

	long date_id;
	int year;
	int month;
	int day;
	struct date* next;

} date;

typedef struct time {

	long time_id;
	int hour;
	int minute;
	struct time* next;

} time;

typedef struct location {

	long location_id;
	char city[TEXT_SHORT];
	char state[TEXT_SHORT];
	struct location* next;

} location;

#endif
