#pragma once

#include <stdint.h>

typedef struct dict_entry {
  char *key;
  void *value;
  struct dict_entry *next;
} dict_entry;

#define HASH_MAP_SIZE 100

typedef struct {
  dict_entry *buckets[HASH_MAP_SIZE];
} dict;

dict *create_dict();

void dict_insert(dict *dict, char *key, void *value);

void *dict_get(dict *dict, char *key);

void free_dict(dict *dict);

#define DICT_FOR_EACH(item, dict)                                              \
  for (int i = 0; i < HASH_MAP_SIZE; i++)                                      \
    for (dict_entry *item = (dict)->buckets[i]; item; item = item->next)
