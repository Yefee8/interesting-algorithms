#ifndef LIST_H
#define LIST_H

typedef struct List {
  int data;
  struct List *next;
} List;

void push_with_order(List **activeQueue, int data);
void free_list(List *head);

#endif
