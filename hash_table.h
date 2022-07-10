#ifndef HASH_TABLE_
#define HASH_TABLE_
#include <stdbool.h>
#include <stdlib.h>
#define HASHTABLE_DEFAULT_SZ 2048

typedef unsigned long hashtype;
typedef struct Hnode Hnode;
typedef struct HashTable HashTable;

Hnode *hnode_alloc(char*);
HashTable *hashtable_alloc();
bool hashtable_put(HashTable*, char*);
bool hashtable_contains(HashTable*, char*);
void hashtable_free(HashTable*);
void hashtable_dump(HashTable*);

#endif // HASH_TABLE_
