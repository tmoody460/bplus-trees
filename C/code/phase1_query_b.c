#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/times.h>
#include <sys/time.h>

#include "tables.h"
#include "query_helper.h"

int main(int argc, char **argv){
	/* start time */
	struct timeval time_start, time_end;
	gettimeofday(&time_start, NULL);
	//query logic start

	if (argc < 3){
		printf("Usage: #num_times #num_messages\n");
		exit(0);
	}

	int num_times = atoi(argv[1]);
	int num_messages = atoi(argv[2]);
	
	int i;
	int user_count;

	int times_end = -1;
	int times_start = -1;

	message_table_entry_t* message_read;
	//binary search timeids
	char filename[TEXT_SHORT];
	FILE *fp = NULL;
	int first = 0, last = num_times-1, middle = (first + last)/2;
	sendtime_table_entry_t* time_read = (sendtime_table_entry_t *)malloc(sizeof(sendtime_table_entry_t));
	sendtime_table_entry_t* time_previous = (sendtime_table_entry_t *)malloc(sizeof(sendtime_table_entry_t));
	while (first <= last){
		read_sendtime_better(middle, time_read);
		if (time_read->hour == 8 || (time_read->hour == 9 && time_read->minute == 0)){
			if (time_read->sendtime_id > times_end){
				times_end = time_read->sendtime_id;
			}
			read_sendtime_better(middle-1, time_previous);
			if (time_previous->hour != 8){
				times_start = middle;
				first = last + 1;
			} else {
				last = middle - 1;
			}
		}else if (time_read->hour < 8){
			first = middle + 1;
		}else {
			last = middle - 1;
		}
		middle = (first + last)/2;
	}
	if (times_start == -1){
		printf("\nCouldn't find any times!\n");
		exit(0);
	}
	free(time_previous);
	
	read_sendtime_better(times_end+1, time_read);
	while (time_read->hour == 8 || (time_read->hour == 9 && time_read->minute == 0)){
		times_end++;
		read_sendtime_better(times_end+1, time_read);
	}
	
	free(time_read);
	int last_user_read = -1;

	message_read = (message_table_entry_t *)malloc(sizeof(message_table_entry_t));

	for (i = 0; i < num_messages; i++){
		if(i%1000 == 0){
			printf("Reading message %d of %d\n", i, num_messages);
		}
		read_message_better(i, message_read);
		if (message_read->sendtime_id >= times_start && message_read->sendtime_id <= times_end){
			if (message_read->user_id > last_user_read){
				last_user_read = message_read->user_id;
				user_count++;
			}
		}
	}
	free(message_read);
	
	//query logic end
	/* end time */
    gettimeofday(&time_end, NULL);
    
    float totaltime = (time_end.tv_sec - time_start.tv_sec)
                    + (time_end.tv_usec - time_start.tv_usec) / 1000000.0f;
                    
    printf("\nThere are %d users who sent messages between 8 and 9.", user_count);
    printf("\n\nProcess time %f seconds\n", totaltime);
    
    return 0;
}
