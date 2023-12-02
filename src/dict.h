#pragma once

#include <stdint.h>

// Forward declaration of be_node from bencoding.h
struct be_node;

typedef struct dict_entry {
  char *key;
  struct be_node *value;
  struct dict_entry *next;
} dict_entry;

#define HASH_MAP_SIZE 100

typedef struct {
  dict_entry *buckets[HASH_MAP_SIZE];
} dict;

dict *create_dict();

void insert(dict *dict, char *key, struct be_node *value);

struct be_node *get(dict *dict, char *key);

void free_dict(dict *dict);
