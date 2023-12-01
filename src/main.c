#include "bencoding.h"
#include <stdio.h>

int main(int argc, char **argv) {
	char *input = argv[1];
	printf("INPUT: %s\n", input);
	be_node *n_int = parse_be_stream(input);

	if (n_int == NULL) {
		printf("string is invalid");
		return 1;
	}

	pretty_print_tree(n_int);
	return 0;
}
