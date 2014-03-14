#ifndef __TABLES_H__
#define __TABLES_H__

#include <stdio.h>
#include <stdlib.h>

#define TEXT_SHORT      64
#define TEXT_LONG       1024

/* message structure */
typedef struct {
    char text[TEXT_LONG];       /* text */
    int year;                   /* the send time of the message: */    
    int month;                  /* month/day/year hour:minute */
    int day;
    int hour;                   /* 0 - 23 */
    int minute;                 /* 0 - 59 */
} message_t;


/* record structure */
typedef struct {
    int id;                     /* user id */                                      
    char name[TEXT_SHORT];      /* user name */
    char location[TEXT_SHORT];  /* user location */
    int message_num;            /* number of send message */
    message_t *messages;        /* messages */
} record_t;


/* user_table_entry_t structure */
typedef struct {
    int  user_id;                    /* user id */                                      
    char user_name[TEXT_SHORT];      /* user name */
    int  location_id;                /* location id */
} user_table_entry_t;


/* message_table_entry_t structure */
typedef struct {
    int message_id;                     /* message id */
    int user_id;                        /* user id */
    char text[TEXT_LONG];               /* text */
    int senddate_id;                /* senddatetime id */
	int sendtime_id;
} message_table_entry_t;


/* location_table_entry_t structure */
typedef struct {
    int location_id;                    /* location id */
    char city[TEXT_SHORT];          /* user location */
	char state[TEXT_SHORT];
} location_table_entry_t;


/* senddate_entry_t structure */
typedef struct {
    int senddate_id;                /* senddatetime id */
    int year;                           /* the send time of the message: */    
    int month;                          /* month/day/year hour:minute */
    int day;
} senddate_table_entry_t;

/*sendtime_entry_t structure*/
typedef struct {
	int sendtime_id;
	int hour;
	int minute;
} sendtime_table_entry_t;

/**
 * read a record from a file
 */
record_t *read_record(FILE *fp);

/**
 * free memory of a record
 */
void free_record(record_t *record);


/**
 * read a user from a file
 */
void read_user(FILE *fp, user_table_entry_t *user);


/**
 * read a message from a file
 */
void read_message(FILE *fp, message_table_entry_t *message);


/**
 * read a location from a file
 */
void read_location(FILE *fp, location_table_entry_t *location);


/**
 * read a senddatetime from a file
 */
void read_senddate(FILE *fp, senddate_table_entry_t *senddate);

void read_sendtime(FILE *fp, sendtime_table_entry_t *sendtime);


/**
 * write a user to a file
 */
void write_user(FILE *fp, user_table_entry_t *user);


/**
 * write a message to a file
 */
void write_message(FILE *fp, message_table_entry_t *message);


/**
 * write a location to a file
 */
void write_location(FILE *fp, location_table_entry_t *location);


/**
 * write a senddatetime to a file
 */
void write_senddate(FILE *fp, senddate_table_entry_t *senddate);

void write_sendtime(FILE *fp, sendtime_table_entry_t *sendtime);

/**
 * print a user from a file
 */
void print_user(user_table_entry_t *user);


/**
 * print a message from a file
 */
void print_message(message_table_entry_t *message);


/**
 * print a location from a file
 */
void print_location(location_table_entry_t *location);


/**
 * print a senddatetime from a file
 */
void print_senddate(senddate_table_entry_t *senddate);

void print_sendtime(sendtime_table_entry_t *sendtime);

void free_users(user_table_entry_t *head);

void free_locations(location_table_entry_t *head);

void free_messages(message_table_entry_t *head);

void free_senddates(senddate_table_entry_t *head);

void free_sendtimes(sendtime_table_entry_t *head);



#endif
