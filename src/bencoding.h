#pragma once

#include <stdint.h>

typedef enum { UNKNOWN, STRING, NUMBER, LIST, DICT } be_type;

// Forward declaration of dict from dict.h
struct dict;

typedef struct be_node {
  be_type type;

  union {
    int64_t int_data;
    char *str_data;
    struct be_node **list_data;
    struct dict *dict_data;
  } data;

} be_node;

be_node *parse_be_stream(char *be_string);

void be_node_free(be_node *node);

void pretty_print_tree(be_node *root);
