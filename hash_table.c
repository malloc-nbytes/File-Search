#include "hash_table.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Hnode {
  char *data;
  Hnode *next;
};

struct HashTable {
  Hnode **table;
  size_t sz;
};

hashtype hash(char *str, int max) {
  hashtype hash = 5381;
  int c;
  while ((c = *str++)) {
    hash = ((hash << 5) + hash) + c;
  }
  return hash % max;
}

Hnode *hnode_alloc(char *data) {
  Hnode *new_node;
  if (!(new_node = (Hnode *)malloc(sizeof(Hnode)))) {
    return NULL;
  }
  new_node->data = data;
  new_node->next = NULL;
  return new_node;
}

HashTable *hashtable_alloc() {
  HashTable *new_hash_table;
  if (!(new_hash_table = (HashTable *)malloc(sizeof(HashTable)))) {
    return NULL;
  }
  if (!(new_hash_table->table =
            (Hnode **)malloc(sizeof(Hnode *) * HASHTABLE_DEFAULT_SZ))) {
    return NULL;
  }
  memset(new_hash_table->table, '\0', sizeof(Hnode *) * HASHTABLE_DEFAULT_SZ);
  new_hash_table->sz = 0;
  return new_hash_table;
}

bool hashtable_contains(HashTable *self, char *data) {
  hashtype id = hash(data, HASHTABLE_DEFAULT_SZ);
  if (!self->table[id]) {
    return false;
  }
  if (strcmp(self->table[id]->data, data) == 0) {
    return true;
  } else {
    Hnode *tmp = self->table[id];
    while (tmp->next) {
      if (strcmp(tmp->next->data, data) == 0) {
        return true;
      }
      tmp = tmp->next;
    }
  }
  return false;
}

bool hashtable_put(HashTable *self, char *data) {
  hashtype id = hash(data, HASHTABLE_DEFAULT_SZ);
  if (!self->table[id]) {
    self->table[id] = hnode_alloc(data);
    self->sz++;
  } else if (strcmp(self->table[id]->data, data) == 0) {
    return false;
  } else {
    Hnode *tmp = self->table[id];
    while (tmp->next) {
      if (strcmp(tmp->next->data, data) == 0) {
        return false;
      }
      tmp = tmp->next;
    }
    tmp->next = hnode_alloc(data);
    self->sz++;
  }
  return true;
}

void hashtable_dump(HashTable *self) {
  for (size_t i = 0; i < HASHTABLE_DEFAULT_SZ; i++) {
    if (self->table[i]) {
      Hnode *tmp = self->table[i];
      while (tmp) {
        Hnode *tmp2 = tmp;
        tmp = tmp->next;
        printf("%s\n", tmp2->data);
      }
    }
  }
}

void hashtable_free(HashTable *self) {
  for (size_t i = 0; i < HASHTABLE_DEFAULT_SZ; i++) {
    if (self->table[i]) {
      Hnode *tmp = self->table[i];
      while (tmp) {
        Hnode *tmp2 = tmp;
        tmp = tmp->next;
        free(tmp2);
      }
    }
  }
  free(self->table);
  free(self);
}
