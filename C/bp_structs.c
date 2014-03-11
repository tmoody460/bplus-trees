#ifndef __BP_STRUCTS__
#define __BP_STRUCTS__

#define FAN_OUT 3
#define NUM_KEYS (FAN_OUT-1)

#define TRUE 1
#define FALSE 0

#include "record.h"

typedef struct node {
	
	char filename[TEXT_SHORT];
	char children[FAN_OUT][TEXT_SHORT];
	char parent[TEXT_SHORT];
	long keys[NUM_KEYS];
	int is_leaf;
	int num_filled;

} node;

#endif