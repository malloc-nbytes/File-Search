#include "stack.h"
#include <stdbool.h>
#include <stdio.h>

struct Node {
  char *data;
  Node *next;
};

struct Stack {
  Node *head;
  size_t sz;
};

void stack_panic(char *err_msg) {
  fprintf(stderr, "ERROR: %s\n", err_msg);
  exit(1);
}

void stack_validate(void *self) {
  if (!self) {
    stack_panic("either stack or stack node failed to stack_validate.");
  }
}

Stack *stack_alloc() {
  Stack *s;
  if (!(s = (Stack *)malloc(sizeof(Stack)))) {
      return NULL;
  }
  s->sz = 0;
  s->head = NULL;
  return s;
}

Node *node_alloc(char *data) {
  Node *n;
  if (!(n = (Node *)malloc(sizeof(Node)))) {
    return NULL;
  }
  n->data = data;
  n->next = NULL;
  return n;
}

void stack_push(Stack *self, char *data) {
  stack_validate(self);
  Node *new_node = node_alloc(data);
  stack_validate(new_node);
  if (!self->head) {
    self->head = new_node;
  }
  else {
    Node *tmp = self->head;
    self->head = new_node;
    self->head->next = tmp;
  }
  self->sz++;
}

char *stack_pop(Stack *self) {
  stack_validate(self);
  char *res = self->head->data;
  Node *tmp = self->head;
  self->head = self->head->next;
  free(tmp);
  self->sz--;
  return res;
}

void stack_dump(Stack *self) {
  stack_validate(self);
  Node *tmp = self->head;
  while (tmp) {
    printf("%s\n", tmp->data);
    tmp = tmp->next;
  }
}

char *stack_peek(Stack *self) {
  stack_validate(self);
  if (!self->head) {
    return NULL;
  }
  return self->head->data;
}

size_t stack_sz(Stack *self) {
  stack_validate(self);
  return self->sz;
}

bool stack_empty(Stack *self) {
  stack_validate(self);
  return !self->head;
}

void stack_free(Stack *self) {
  stack_validate(self);
  while (self->head) {
    Node *tmp = self->head;
    self->head = self->head->next;
    free(tmp);
  }
  free(self);
}

