#include "bencoding.h"
#include <stdio.h>

int main(int argc, char **argv) {
	// be_node *node  = parse_be_stream("4:spam");
	// be_node *p_int = parse_be_stream("i3e");
	//
	char *input = "li-3ell4:spami3eeee";
	printf("INPUT: %s\n", input);
	be_node *n_int = parse_be_stream(input);

	if (n_int == NULL) {
		printf("string is invalid");
		return 1;
	}

	pretty_print_tree(n_int);
	// be_node nested_nested_node = ((be_node *)nested_node.data)[0];
	// be_node nested_nested_nested_node = ((be_node
	// *)nested_nested_node.data)[0];

	// printf("node is of type %d and data is %s\n", node->type, (char
	// *)node->data); printf("p_int is of type %d and data is %d\n",
	// p_int->type, *(int *)p_int->data);
	return 0;
}
