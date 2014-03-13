#include "tables.h"


/**
 * read a message from a file
 */
void read_record_message(message_t *message, FILE *fp)
{
    /* Assume file has been opened */
    if (fp == NULL) {
        fprintf(stderr, "The file stream is NULL\n");
        exit(0);
    }
    
    /* message cannot be NULL */
    if (message == NULL) {
        fprintf(stderr, "The message is NULL\n");
        exit(0);
    }
    
    /* read message text */
    fread(&(message->text[0]), sizeof(char), TEXT_LONG, fp);
    
    /* read message time */
    fread(&(message->year), sizeof(int), 1, fp);
    fread(&(message->month), sizeof(int), 1, fp);
    fread(&(message->day), sizeof(int), 1, fp);
    fread(&(message->hour), sizeof(int), 1, fp);
    fread(&(message->minute), sizeof(int), 1, fp);
}


/**
 * read a record from a file
 */
record_t *read_record(FILE *fp) 
{
    int i;      
    
    /* Assume file has been opened */
    if (fp == NULL) {
        fprintf(stderr, "The file stream is NULL\n");
        exit(0);
    }
    
    /* allocate memory for the record */
    record_t *record = (record_t *)malloc(sizeof(record_t));
    
    /* memory error */
    if (record == NULL) {
        fprintf(stderr, "Cannot allocate memory for record.\n");
        exit(0);
    }
    
    /* read user id */
    fread(&(record->id), sizeof(int), 1, fp);
    
    /* read user name */
    fread(&(record->name[0]), sizeof(char), TEXT_SHORT, fp);
    
    /* read user location */
    fread(&(record->location[0]), sizeof(char), TEXT_SHORT, fp);
    
    /* read message number */
    fread(&(record->message_num), sizeof(int), 1, fp);
    
    /* initalize messsages */
    record->messages = NULL;
    
    /* allocate memory for messages if the message number is greater than 0 */
    if (record->message_num > 0) {
        
        /*allocate memory */
        record->messages = (message_t *)malloc(sizeof(message_t) * 
                                               record->message_num);
        
        /* memory error */
        if (record->messages == NULL) {
            fprintf(stderr, "Cannot allocate memory for messages.\n");
            exit(0);    
        }
        
        /* read each message from file */
        for(i = 0; i < record->message_num; i++) {
            read_record_message(&(record->messages[i]), fp);
        }
    }
    
    /* return the record */
    return record;
}

/**
 * free memory of a record
 */
void free_record(record_t *record)
{
    if (record == NULL) {
        return;
    }
 
    /* free message memory */
    if (record->messages != NULL) {
        free(record->messages);
    }
    
    /* free record memory */
    free(record);
}


/**
 * read a user from a file
 */
void read_user(FILE *fp, user_table_entry_t *user)
{
    /* Assume file has been opened */
    if (fp == NULL) {
        fprintf(stderr, "The file stream is NULL\n");
        exit(0);
    }
    
    /* memory error */
    if (user == NULL) {
        fprintf(stderr, "Cannot allocate memory for user.\n");
        exit(0);
    }
    
    /* read a user */
    fread(user, sizeof(user_table_entry_t), 1, fp);
}


/**
 * read a message from a file
 */
void read_message(FILE *fp, message_table_entry_t *message)
{
    /* Assume file has been opened */
    if (fp == NULL) {
        fprintf(stderr, "The file stream is NULL\n");
        exit(0);
    }
    
    /* memory error */
    if (message == NULL) {
        fprintf(stderr, "Cannot allocate memory for message.\n");
        exit(0);
    }
    
    /* read message*/
    fread(message, sizeof(message_table_entry_t), 1, fp);
}


/**
 * read a location from a file
 */
void read_location(FILE *fp, location_table_entry_t *location)
{
    /* Assume file has been opened */
    if (fp == NULL) {
        fprintf(stderr, "The file stream is NULL\n");
        exit(0);
    }
    
    /* memory error */
    if (location == NULL) {
        fprintf(stderr, "Cannot allocate memory for location.\n");
        exit(0);
    }
    
    /* read location */
    fread(location, sizeof(location_table_entry_t), 1, fp);
}


/**
 * read a senddatetime from a file
 */
void read_senddate(FILE *fp, senddate_table_entry_t *senddate)
{
    /* Assume file has been opened */
    if (fp == NULL) {
        fprintf(stderr, "The file stream is NULL\n");
        exit(0);
    }
    
    /* memory error */
    if (senddate == NULL) {
        fprintf(stderr, "Cannot allocate memory for senddate.\n");
        exit(0);
    }
    
    /* read senddate */
    fread(senddate, sizeof(senddate_table_entry_t), 1, fp);
}

void read_sendtime(FILE *fp, sendtime_table_entry_t *sendtime)
{
	/* Assume file has been opened */
    if (fp == NULL) {
        fprintf(stderr, "The file stream is NULL\n");
        exit(0);
    }
    
    /* memory error */
    if (sendtime == NULL) {
        fprintf(stderr, "Cannot allocate memory for sendtime.\n");
        exit(0);
    }
    
    /* read sendtime */
    fread(sendtime, sizeof(sendtime_table_entry_t), 1, fp);	
}



/**
 * write a user to a file
 */
void write_user(FILE *fp, user_table_entry_t *user)
{
    /* Assume file has been opened */
    if (fp == NULL) {
        fprintf(stderr, "The file stream is NULL\n");
        exit(0);
    }
    
    /* memory error */
    if (user == NULL) {
        fprintf(stderr, "The memory is NULL.\n");
        exit(0);
    }
    
    /* write user */
    fwrite(user, sizeof(user_table_entry_t), 1, fp);
}


/**
 * write a message to a file
 */
void write_message(FILE *fp, message_table_entry_t *message)
{
    /* Assume file has been opened */
    if (fp == NULL) {
        fprintf(stderr, "The file stream is NULL\n");
        exit(0);
    }
    
   
    /* memory error */
    if (message == NULL) {
        fprintf(stderr, "The memory is NULL.\n");
        exit(0);
    }
    
    /* write message */
    fwrite(message, sizeof(message_table_entry_t), 1, fp);
}


/**
 * write a location to a file
 */
void write_location(FILE *fp, location_table_entry_t *location)
{
    /* Assume file has been opened */
    if (fp == NULL) {
        fprintf(stderr, "The file stream is NULL\n");
        exit(0);
    }
    
    /* memory error */
    if (location == NULL) {
        fprintf(stderr, "The memory is NULL.\n");
        exit(0);
    }
    
    /* write location*/
    fwrite(location, sizeof(location_table_entry_t), 1, fp);
}


/**
 * write a senddate to a file
 */
void write_senddate(FILE *fp, senddate_table_entry_t *senddate)
{
    /* Assume file has been opened */
    if (fp == NULL) {
        fprintf(stderr, "The file stream is NULL\n");
        exit(0);
    }
    
    /* memory error */
    if (senddate == NULL) {
        fprintf(stderr, "The memory is NULL.\n");
        exit(0);
    }
    
    /* write senddatetime*/
    fwrite(senddate, sizeof(senddate_table_entry_t), 1, fp);
}

void write_sendtime(FILE *fp, sendtime_table_entry_t *sendtime)
{
    /* Assume file has been opened */
    if (fp == NULL) {
        fprintf(stderr, "The file stream is NULL\n");
        exit(0);
    }
    
    /* memory error */
    if (sendtime == NULL) {
        fprintf(stderr, "The memory is NULL.\n");
        exit(0);
    }
    
    /* write senddatetime*/
    fwrite(sendtime, sizeof(sendtime_table_entry_t), 1, fp);
}

/**
 * print a user
 */
void print_user(user_table_entry_t *user)
{
    /* record cannot be NULL */
    if (user == NULL) {
        fprintf(stderr, "The record is NULL\n");
        exit(0);
    }
    
    /* print user id */
    printf("User ID: %07d\n", user->user_id);
    
    /* print user name */
    printf("User Name: %s\n", user->user_name);
    
    /* print user location */
    printf("Location ID: %07d\n", user->location_id);
}


/**
 * print a message
 */
void print_message(message_table_entry_t *message)
{
    /* record cannot be NULL */
    if (message == NULL) {
        fprintf(stderr, "The record is NULL\n");
        exit(0);
    }
    
    /* print message id */
    printf("Message ID: %07d\n", message->message_id);
    
    /* print user id */
    printf("User ID: %07d\n", message->user_id);
    
    /* print senddate id */
    printf("SendDate ID: %07d\n", message->senddate_id);

    /* print sendtime id */
    printf("SendTime ID: %07d\n", message->sendtime_id);
          
    /* print message */
    printf("Text: %s\n", message->text);       
}


/**
 * print a location
 */
void print_location(location_table_entry_t *location)
{
    /* record cannot be NULL */
    if (location == NULL) {
        fprintf(stderr, "The record is NULL\n");
        exit(0);
    }
    
    /* print location id */
    printf("Location ID: %07d\n", location->location_id);
          
    /* print location */
    printf("Location: %s, %s\n", location->city, location->state);       
}



/**
 * print a senddatetime
 */
void print_senddate(senddate_table_entry_t *senddate)
{
    /* record cannot be NULL */
    if (senddate == NULL) {
        fprintf(stderr, "The record is NULL\n");
        exit(0);
    }
    
    /* print senddatetime id */
    printf("Senddate ID: %07d\n", senddate->senddate_id);
          
    /* print time */
       /* print message time */
    printf("Date: %02d/%02d/%04d", senddate->month, senddate->day, 
           senddate->year);    
}

void print_sendtime(sendtime_table_entry_t *sendtime)
{
    /* record cannot be NULL */
    if (sendtime == NULL) {
        fprintf(stderr, "The record is NULL\n");
        exit(0);
    }
    
    /* print sendtime id */
    printf("Sendtime ID: %07d\n", sendtime->sendtime_id);
          
    /* print time */
       /* print message time */
    printf("Time: %02d:%02d\n", sendtime->hour, sendtime->minute);    
}




