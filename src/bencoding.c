// Bencoding is a way to specify and organize data in a terse format.
// It supports the following types: byte strings, integers, lists, and
// dictionaries.

#include "bencoding.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct be_scanner {
	char *start;
	char *current;
} be_scanner;

static be_node *parse_be_list(be_scanner *scanner);

static void be_scanner_free(be_scanner *scanner) {
	free(scanner->current);
	free(scanner);
}

static char advance(be_scanner *scanner) {
	scanner->current++;
	return scanner->current[-1];
}

static char peek(be_scanner *scanner) { return *scanner->current; }

static void debug_scanner(be_scanner *scanner, char *extra) {
	printf("-----\n");
	printf("extra: %s\n", extra);
	printf("scanner->start: %s\n", scanner->start);
	printf("scanner->current: %s\n", scanner->current);
	printf("peek '%c'\n", peek(scanner));
	printf("-----\n");
}

static bool isAtEnd(be_scanner *scanner) { return *scanner->current == '\0'; }

static bool match(be_scanner *scanner, char expected) {
	if (isAtEnd(scanner)) {
		return false;
	}
	if (*scanner->current != expected) {
		return false;
	}
	scanner->current++;
	return true;
}

static be_node *create_be_node(be_type type) {
	be_node *node = malloc(sizeof(be_node));
	*node		  = (be_node){.type = type};

	return node;
}

void be_node_free(be_node *node) {
	if (node->type == LIST) {
		if (node->data.list_data != NULL) {
			for (int i = 0; node->data.list_data[i] != NULL; i++) {
				be_node_free(node->data.list_data[i]);
			}
		}
		free(node->data.list_data);
	}
	if (node->type == STRING) {
		free(node->data.str_data);
	}
	free(node);
}

static be_node *parse_be_string(be_scanner *scanner) {
	while (isdigit(peek(scanner))) {
		advance(scanner);
	}

	char *str_len = malloc(scanner->current - scanner->start);
	str_len =
		strncpy(str_len, scanner->start, scanner->current - scanner->start);

	int n_chars = strtol(str_len, NULL, 10);
	free(str_len);

	advance(scanner); // Consume the :

	char *value = malloc(n_chars * sizeof(char));
	value		= strncpy(value, scanner->current, n_chars);

	scanner->current += n_chars;

	be_node *node		= create_be_node(STRING);
	node->data.str_data = value;

	return node;
}

static be_node *parse_be_number(be_scanner *scanner) {
	if (!match(scanner, 'i')) {
		return NULL;
	}

	char *begin_of_number = scanner->current;

	while (peek(scanner) != 'e') {
		advance(scanner);
	}

	char *number_str = malloc(scanner->current - begin_of_number);
	number_str		 = strncpy(number_str, begin_of_number,
							   scanner->current - begin_of_number);

	int64_t length = strlen(number_str);

	bool has_leading_zeros	= length > 1 && *number_str == '0';
	bool invalid_neg_number = strncmp(number_str, "-0", 2) == 0;

	if (has_leading_zeros || invalid_neg_number) {
		return NULL;
	}

	int64_t number = strtol(number_str, NULL, 10);

	// Advance the 'e'
	advance(scanner);

	be_node *node		= create_be_node(NUMBER);
	node->data.int_data = number;

	return node;
}

static be_node *parse_be(be_scanner *scanner) {
	be_node *root  = NULL;
	scanner->start = scanner->current;

	while (!isAtEnd(scanner)) {
		char c = peek(scanner);
		if (isdigit(c)) {
			root = parse_be_string(scanner);
		}
		if (c == 'i') {
			root = parse_be_number(scanner);
		}
		if (c == 'l') {
			root = parse_be_list(scanner);
		}
	}
	return root;
}

static be_node *parse_be_list(be_scanner *scanner) {
	be_node *node		 = create_be_node(LIST);
	node->data.list_data = NULL;

	if (!match(scanner, 'l')) {
		return NULL;
	}

	if (match(scanner, 'e')) {
		return node;
	}

	int64_t	  capacity = 1;
	be_node **nodes	   = malloc(sizeof(be_node *) * capacity);

	int64_t index = 0;
	while (!match(scanner, 'e') && !isAtEnd(scanner)) {
		scanner->start = scanner->current;
		char c		   = peek(scanner);

		if (isdigit(c)) {
			nodes[index++] = parse_be_string(scanner);
		}
		if (c == 'i') {
			nodes[index++] = parse_be_number(scanner);
		}
		if (c == 'l') {
			nodes[index++] = parse_be_list(scanner);
		}

		// Grow array with twice the cap
		if (index == capacity) {
			capacity *= 2;
			nodes = (be_node **)realloc(nodes, capacity * sizeof(be_node *));
			if (nodes == NULL) {
				fprintf(stderr, "Memory reallocation failed\n");
				return NULL;
			}
		}
	}

	advance(scanner);

	// Add null terminator at the end
	nodes[index]		 = NULL;
	node->data.list_data = nodes;

	return node;
}

be_node *parse_be_stream(char *be_string) {
	char	   *be_string_dup = strdup(be_string);
	be_scanner *scanner = &(be_scanner){.start = 0, .current = be_string_dup};
	be_node	   *node	= parse_be(scanner);

	free(be_string_dup);

	return node;
}

void pretty_print_node(be_node *node, int indent) {
	switch (node->type) {
	case STRING:
		printf("Type: String - Value: %s\n", node->data.str_data);
		break;
	case NUMBER:
		printf("Type: Number - Value: %ld\n", node->data.int_data);
		break;
	case LIST:
		printf("Type: List - Value: \n");
		be_node **data = node->data.list_data;
		if (data == NULL) {
			return;
		}

		for (int i = 0; data[i] != NULL; i++) {
			printf("%*s|- ", indent, "");
			pretty_print_node(data[i], indent + 3);
		}
		break;
	case DICT:
		printf("Type: Dict - ");
		break;
	default:
		printf("Unknown type %d\n", node->type);
		break;
	}
}

void pretty_print_tree(be_node *root) { pretty_print_node(root, 0); }
