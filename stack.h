#ifndef STACK_H_
#define STACK_H_
#include <stdbool.h>
#include <stdlib.h>

typedef struct Stack Stack;
typedef struct Node Node;

Stack *stack_alloc();
Node *node_alloc(char*);
void stack_push(Stack*, char*);
char *stack_pop(Stack*);
char *stack_peek(Stack*);
bool stack_empty(Stack*);
size_t stack_sz(Stack*);
void stack_dump(Stack*);
void stack_free(Stack*);

#endif // STACK_H_
