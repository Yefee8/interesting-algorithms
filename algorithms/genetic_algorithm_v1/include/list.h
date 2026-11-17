#ifndef LIST_H
#define LIST_H

typedef struct List {
  int data;
  struct List *next;
} List;

void push_with_order(List **set, int data);
int find_data_by_index(List *list, int index);
void free_set(List *head);

#endif
