#include "bp_structs.c"
#include "structures.h"
#include <string.h>
#include "rw_methods.c"

#define TRUE 1
#define FALSE 0
#define ROOT_PATH "user_tree/root.dat"

int main(int argc, char** argv) {

	//http://dblab.cs.toronto.edu/courses/443/2014/05.btree-index.html
	if(argc != 2) {
		fprintf(stderr, "Usage: %s node_file", argv[0]);
		exit(0);
	}

	node* to_print = (node*) malloc(sizeof(node));
	read_node(to_print, argv[1]);
	print_node(to_print);
	free(to_print);
}