#include "bencoding.h"
#include "dict.h"
#include <stdio.h>

int main(int argc, char **argv) {
  char *input = argv[1];
  printf("INPUT: %s\n", input);
  be_node *node = parse_be_stream(input);

  if (node == NULL) {
    printf("string is invalid");
    return 1;
  }

  dict *map = create_dict();
  insert(map, "hello", node);
  be_node *item = get(map, "hello");

  pretty_print_tree(item);

  be_node_free(node);
  free_dict(map);
  return 0;
}
