#pragma once

#include "bencoding.h"
#include <stdint.h>

typedef struct dict_entry {
	char			  *key;
	be_node			  *value;
	struct dict_entry *next;
} dict_entry;

#define HASH_MAP_SIZE 100

typedef struct {
	dict_entry *buckets[HASH_MAP_SIZE];
} dict;

void insert(dict *dict, char *key, be_node *value);

be_node *get(dict *dict, char *key);

void free_dict(dict *dict);
