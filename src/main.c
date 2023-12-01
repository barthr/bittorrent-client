#include "bencoding.h"
#include <stdio.h>

int main(int argc, char **argv) {
	char *input = argv[1];
	printf("INPUT: %s\n", input);
	be_node *node = parse_be_stream(input);

	if (node == NULL) {
		printf("string is invalid");
		return 1;
	}

	pretty_print_tree(node);
	be_node_free(node);
	return 0;
}
