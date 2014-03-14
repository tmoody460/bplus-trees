#ifndef __BP_STRUCTS__
#define __BP_STRUCTS_LOCATION__

#define FAN_OUT 3
#define NUM_KEYS (FAN_OUT-1)

#define TRUE 1
#define FALSE 0
#define TEXT_SHORT 64
#define ROOT_PATH "../data/user_tree/root.dat"

typedef struct node {
	
	char filename[TEXT_SHORT];
	char children[FAN_OUT][TEXT_SHORT];
	char parent[TEXT_SHORT];
	char keys[NUM_KEYS][TEXT_SHORT];
	int is_leaf;
	int num_filled;

} node;

#endif