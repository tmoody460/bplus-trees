#include "bp_structs_user.c"
#include <string.h>
#include "rw_methods_user.c"

#define TRUE 1
#define FALSE 0

int main(int argc, char** argv) {

	if(argc != 2) {
		fprintf(stderr, "Usage: %s node_file", argv[0]);
		exit(0);
	}

	user_node* to_print = (user_node*) malloc(sizeof(node));
	read_user_node(to_print, argv[1]);
	print_user_node(to_print);
	free(to_print);
}