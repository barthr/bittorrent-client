#pragma once

#include "dict.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

dict *create_dict() {
  dict *dictionary = (dict *)malloc(sizeof(dict));
  memset(dictionary->buckets, 0, sizeof(dictionary->buckets));

  return dictionary;
}

void free_dict(dict *dict) {
  for (int i = 0; i < HASH_MAP_SIZE; i++) {
    dict_entry *current = dict->buckets[i];
    while (current != NULL) {
      dict_entry *next = current->next;
      free(current->key);
      free(current);
      current = next;
    }
  }
}

static unsigned int hash(char *key) {
  unsigned int hash = 0;
  while (*key) {
    hash = (hash << 5) + *key++;
  }
  return hash % HASH_MAP_SIZE;
}

static dict_entry *create_dict_entry(char *key, struct be_node *value) {
  dict_entry *entry = (dict_entry *)malloc(sizeof(struct dict_entry));

  entry->key = strdup(key);
  entry->value = value;
  entry->next = NULL;

  return entry;
}

void insert(dict *dict, char *key, struct be_node *value) {
  unsigned int index = hash(key);
  dict_entry *entry = create_dict_entry(key, value);

  if (dict->buckets[index] == NULL) {
    dict->buckets[index] = entry;
  } else {
    entry->next = dict->buckets[index];
    dict->buckets[index] = entry;
  }
}

struct be_node *get(dict *map, char *key) {
  unsigned int index = hash(key);
  dict_entry *current = map->buckets[index];

  while (current != NULL) {
    if (strcmp(current->key, key) == 0) {
      return current->value;
    }
    current = current->next;
  }

  return NULL;
}
