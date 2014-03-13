#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "record.h"
#include "structures.h"

//arg[1] is number of users
//arg[2] is number of messages
//arg[3] is number of locations
//arg[4] is number of dates
//arg[5] is number of times
int main(int argc, char **argv){
	/* start time */
	struct timeval time_start, time_end;
	gettimeofday(&time_start, NULL);
	//query logic start
	int num_users = atoi(argv[1]);
	int num_messages = atoi(argv[2]);
	int num_locations = atoi(argv[3]);
	int num_dates = atoi(argv[4]);
	int num_times = atoi(argv[5]);
	
	int i;
	int user_count;
	user* user_head=NULL;
	user* user_tail;
	user* user_current;
	int user_found = 0;
	
	time* time_head=NULL;
	time* time_tail;
	time* time_current;
	time* time_read;
	int time_found = 0;

	message* message_read;
	//binary search timeids
	char filename[TEXT_SHORT];
	FILE *fp = NULL;
	int first = 0, last = num_times-1, middle = (first + last)/2;
	time_read = (time *)malloc(sizeof(time));
	time* time_previous = (time *)malloc(sizeof(time));
	int times_start = -1;
	while (first <= last){
		sprintf(filename, "times/time_%07d.dat", middle);
		fp = fopen(filename, "rb");
		read_time(fp, time_read);
		fclose(fp);
		
		if (time_read->hour == 8 || (time_read->hour == 9 && time_read->minute == 0)){
			sprintf(filename, "times/time_%07d.dat", middle-1);
			fp = fopen(filename, "rb");
			read_time(fp, time_previous);
			fclose(fp);
			if (time_previous->hour == 8){
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
	i = times_start;
	int times_done = 0;
	while (times_done = 0){
		sprintf(filename, "times/time_%07d.dat", i);
		fp = fopen(filename, "rb");
		read_time(fp, time_read);
		fclose(fp);
		if (time_read->hour == 8 || (time_read->hour == 9 && time_read->minute == 0)){
			time_current = (time*)malloc(sizeof(time));
			time_current->next = NULL;
			time_current->time_id = time_read->time_id;
			if (time_head == NULL){
				time_head = time_current;
				time_tail = time_current;
			}else{
				time_tail->next = time_current;
				time_tail = time_current;
			}
		}else{
			times_done = 1;
		}
		if (i == num_times){
			times_done = 1;
		}
		i++;
	}
	free(time_read);

	message_read = (message *)malloc(sizeof(message));
	for (i = 0; i < num_messages; i++){
		if(i%1000 == 0){
			printf("Reading message %d of %d\n", i, num_messages);
		}
		sprintf(filename, "messages/message_%07d.dat", i);
		fp = fopen(filename,"rb");
		read_database_message(fp, message_read);
		fclose(fp);
		time_current = time_head;
		while (time_current != NULL && time_found == 0){
			if (time_current->time_id == message_read->time_id){
				time_found = 1;				
				user_current = user_head;
				while (user_current != NULL && user_found == 0){
					if (user_current->user_id == message_read->user_id){
						user_found = 1;
					}
					user_current = user_current->next;
				}
				if (user_found == 0){
				
					user_current = (user *)malloc(sizeof(user));
					user_current->user_id = message_read->user_id;
					user_current->next = NULL;
					if(user_head == NULL){
						user_head = user_current;
						user_tail = user_current;
					}else{
						user_tail->next = user_current;
						user_tail = user_current;
					}
					user_count++;
				}
				user_found = 0;
			}
			time_current = time_current->next;
		}
		time_found = 0;
	}
	free(message_read);
	
	//query logic end
	/* end time */
    gettimeofday(&time_end, NULL);
    
    float totaltime = (time_end.tv_sec - time_start.tv_sec)
                    + (time_end.tv_usec - time_start.tv_usec) / 1000000.0f;
                    
    free_users(user_head);
    free_times(time_head);
    printf("\nThere are %d users who sent messages between 8 and 9.", user_count);
    printf("\n\nProcess time %f seconds\n", totaltime);
    
    return 0;
}
