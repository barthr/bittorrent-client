#include "bencoding.h"
#include <stdio.h>

int main(int argc, char **argv) {
	printf("Hello world: %s\n", "bart");
	// be_node *node  = parse_be_stream("4:spam");
	// be_node *p_int = parse_be_stream("i3e");
	be_node *n_int = parse_be_stream("llee");

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
