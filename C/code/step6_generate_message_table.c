#include <stdio.h>
#include <stdlib.h>
#include <sys/times.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#include "tables.h"

/*
 * the auxiliary data struct
 * 
 */
typedef struct {    
    int year;   
    int month;  
    int day;
} datadate_t;

typedef struct {    
    int hour;   
    int minute; 
} datatime_t;
/*
 * the auxiliary data struct for lookup
 * 
 */
typedef struct {                                                   
    int  senddate_id;               
    datadate_t datadate;
} senddate_lookup_t;

typedef struct {
    int  sendtime_id;               
    datatime_t datatime;
} sendtime_lookup_t;



/*
 * the auxiliary compare function
 * 
 */
int compare_date(const message_t *ta, const datadate_t *tb) 
{
    if (ta->year != tb->year) {
        return ta->year < tb->year? 1 : -1;
    } else if (ta->month != tb->month) {
        return ta->month < tb->month? 1 : -1;
    } else if (ta->day != tb->day) {
        return ta->day < tb->day? 1 : -1;
    }
    
    return 0;
}

int compare_time(const message_t *ta, const datatime_t *tb) 
{
	if (ta->hour != tb->hour) {
        return ta->hour < tb->hour? 1 : -1;
    } else if (ta->minute != tb->minute) {
        return ta->minute < tb->minute? 1 : -1;
    }
    
    return 0;
}


/*
 * search the senddatetime id using binary searh
 * 
 */
int look_up_senddate_id(message_t message, 
                            int total_senddate_number,
                            senddate_lookup_t* senddate_lookup)
{    
    int imin = 0;
    int imax = total_senddate_number - 1;

    while (imax >= imin) {
        int imid = (imax + imin) / 2;     
        int res = compare_date(&message, &(senddate_lookup[imid].datadate));
        if (res == 0) {
            return senddate_lookup[imid].senddate_id;
        } else if (res < 0) {
            imin = imid + 1;
        } else {
            imax = imid;
        }        
    }
    
    fprintf(stderr, "Cannot find the senddate id for message %s\n", message.text);
    return -1;
}

int look_up_sendtime_id(message_t message, 
                            int total_sendtime_number,
                            sendtime_lookup_t* sendtime_lookup)
{    
    int imin = 0;
    int imax = total_sendtime_number - 1;
    
    while (imax >= imin) {
        int imid = (imax + imin) / 2;
        
        int res = compare_time(&message, &(sendtime_lookup[imid].datatime));
        
        if (res == 0) {
            return sendtime_lookup[imid].sendtime_id;
        } else if (res < 0) {
            imin = imid + 1;
        } else {
            imax = imid;
        }        
    }
    
    fprintf(stderr, "Cannot find the sendtime id for message %s\n", message.text);
    return -1;
}

int main(int argc, char **argv)
{
    int i, j;
    struct stat st;
    
    /* print usage if needed */
    if (argc != 4) {
        fprintf(stderr, "Usage: %s total_record_number total_senddate_number total_sendtime_number\n", argv[0]);
        exit(0);
    }
    
    /* get total record number */
    int total_record_number = atoi(argv[1]);
    
    /* get total senddatetime number */
    int total_senddate_number = atoi(argv[2]);

	int total_sendtime_number = atoi(argv[3]);
    
    char filename[1024];
    FILE *fp = NULL;
    
    struct timeval time_start, time_end;
    
    /* start time */
    gettimeofday(&time_start, NULL);
    
    /* prepare the senddate lookup table */
    senddate_lookup_t *senddate_lookup 
        = (senddate_lookup_t *)malloc(sizeof(senddate_lookup_t) 
                                          * total_senddate_number);
    int subdir = 0;    
    senddate_table_entry_t senddate_table_entry;    
    for (i = 0; i < total_senddate_number; i++) {
        /* open the corresponding directory */
        sprintf(filename, "../data/unique_senddate");
        if (stat(filename, &st) == -1) {
            fprintf(stderr, "Cannot open %s\n", filename);
            exit(0);
        } 
        subdir = (i / 100000 + 1) * 100000;
        sprintf(filename, "../data/unique_senddate/%07d", subdir);
        if (stat(filename, &st) == -1) {
            fprintf(stderr, "Cannot open %s\n", filename);
            exit(0);
        }
        
        /* open the corresponding file */  
        sprintf(filename, 
                "../data/unique_senddate/%07d/senddate_%07d.dat", 
                subdir,
                i);
           
        fp = fopen(filename,"rb");
    
        if (!fp) {
            fprintf(stderr, "Cannot open %s\n", filename);
            exit(0);
        } 
        
        /* read the data from the file */
        read_senddate(fp, &senddate_table_entry);
        fclose(fp);  
        
        senddate_lookup[i].senddate_id = senddate_table_entry.senddate_id;
        senddate_lookup[i].datadate.year   = senddate_table_entry.year;
        senddate_lookup[i].datadate.month  = senddate_table_entry.month;
        senddate_lookup[i].datadate.day    = senddate_table_entry.day;
    }

	//prepare the time table
	sendtime_lookup_t *sendtime_lookup 
        = (sendtime_lookup_t *)malloc(sizeof(sendtime_lookup_t) 
                                          * total_sendtime_number);
    subdir = 0;   
    sendtime_table_entry_t sendtime_table_entry;    
    for (i = 0; i < total_sendtime_number; i++) {
        
        /* open the corresponding directory */
        sprintf(filename, "../data/unique_sendtime");
        if (stat(filename, &st) == -1) {
            fprintf(stderr, "Cannot open %s\n", filename);
            exit(0);
        } 
        subdir = (i / 100000 + 1) * 100000;
        sprintf(filename, "../data/unique_sendtime/%07d", subdir);
        if (stat(filename, &st) == -1) {
            fprintf(stderr, "Cannot open %s\n", filename);
            exit(0);
        }
        
        /* open the corresponding file */  
        sprintf(filename, 
                "../data/unique_sendtime/%07d/sendtime_%07d.dat", 
                subdir,
                i);
           
        fp = fopen(filename,"rb");
    
        if (!fp) {
            fprintf(stderr, "Cannot open %s\n", filename);
            exit(0);
        } 
        
        /* read the data from the file */
        read_sendtime(fp, &sendtime_table_entry);
        fclose(fp);  
        
		sendtime_lookup[i].sendtime_id = sendtime_table_entry.sendtime_id;
        sendtime_lookup[i].datatime.hour   = sendtime_table_entry.hour;
        sendtime_lookup[i].datatime.minute = sendtime_table_entry.minute;
    }
    
    message_table_entry_t message_table_entry;    
    int count = 0;
    for (i = 0; i < total_record_number; i++) {
       
        if ((i+1) % 1000 == 0) {
            printf("processing %06d of %06d\n", i+1, total_record_number);
        }
        
        /* open the corresponding file */  
        sprintf(filename, "../data/original/record_%06d.dat", i);
        
        fp = fopen(filename,"rb");
        
        if (!fp) {
            fprintf(stderr, "Cannot open %s\n", filename);
            exit(0);
        }
        
        /* read the record from the file */
        record_t *rp = read_record(fp);
        
        /* close the file */
        fclose(fp);
        
        /* =========== start of data processing code ================ */
        
        for (j = 0; j < rp->message_num; j++) {
            message_table_entry.message_id = count;;
            message_table_entry.user_id = rp->id;
            strncpy(message_table_entry.text, rp->messages[j].text, TEXT_LONG);

            message_table_entry.senddate_id
                    = look_up_senddate_id(rp->messages[j], 
                                              total_senddate_number, 
                                              senddate_lookup);

            message_table_entry.sendtime_id
                    = look_up_sendtime_id(rp->messages[j], 
                                              total_sendtime_number, 
                                              sendtime_lookup);

            /* open the corresponding directory */
            sprintf(filename, "../data/unsorted_message");
            if (stat(filename, &st) == -1) {
                mkdir(filename, 0700);
            } 
            subdir = (count / 100000 + 1) * 100000;
            sprintf(filename, "../data/unsorted_message/%07d", subdir);
            if (stat(filename, &st) == -1) {
                mkdir(filename, 0700);
            }
            
            /* open the corresponding file */  
            sprintf(filename, 
                    "../data/unsorted_message/%07d/message_%07d.dat", 
                    subdir,
                    count);
            fp = fopen(filename,"wb");
            
            if (!fp) {
                fprintf(stderr, "Cannot open %s\n", filename);
                exit(0);
            }
            
            /* write the message to the file */
            write_message(fp, &message_table_entry);
            
            /* close the file */
            fclose(fp);

			count++;
            
        }
        
        /* =========== end of data processing code ================ */    
        
        /* free memory */
        free_record(rp);
    }            
    
    printf("Total generate %d files for the message table\n", count);
    
    free(senddate_lookup);
    free(sendtime_lookup);
    
    /* end time */
    gettimeofday(&time_end, NULL);
    
    float totaltime = (time_end.tv_sec - time_start.tv_sec)
    + (time_end.tv_usec - time_start.tv_usec) / 1000000.0f;
    
    
    printf("\n\nProcess time %f seconds\n", totaltime);
    
    return 0;
}


