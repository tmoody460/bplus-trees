#ifndef __BP_STRUCTS__
#define __BP_STRUCTS__

#define FAN_OUT 3
#define NUM_KEYS (FAN_OUT-1)

#define TRUE 1
#define FALSE 0
#define TEXT_SHORT 64
#define USER_ROOT_PATH "../data/user_tree/root.dat"
#define LOCATION_ROOT_PATH "../data/user_tree/root.dat"

typedef struct user_node {
	
	char filename[TEXT_SHORT];
	char children[FAN_OUT][TEXT_SHORT];
	char parent[TEXT_SHORT];
	long keys[NUM_KEYS];
	int is_leaf;
	int num_filled;

} user_node;

typedef struct location_node {
	
	char filename[TEXT_SHORT];
	char children[FAN_OUT][TEXT_SHORT];
	char parent[TEXT_SHORT];
	char keys[NUM_KEYS][TEXT_SHORT];
	int is_leaf;
	int num_filled;

} location_node;

#endif