#include "list.h"
#include <stdlib.h>

void push_with_order(List **set, int data) {
  List *newItem = calloc(1, sizeof(List));

  if (newItem == NULL)
    return;

  newItem->data = data;
  newItem->next = NULL;

  if (*set == NULL) {
    *set = newItem;
    return;
  }

  List *temp = *set;
  List *lastItem = NULL;

  while (temp != NULL) {
    if (newItem->data >= temp->data) {
      newItem->next = temp;

      if (lastItem != NULL)
        lastItem->next = newItem;
      else
        *set = newItem;

      return;
    }

    lastItem = temp;
    temp = temp->next;
  }

  lastItem->next = newItem;
}

void free_set(List *head) {
  while (head != NULL) {
    List *next = head->next;
    free(head);
    head = next;
  }
}

int find_data_by_index(List *list, int index) {
  int i = 0;
  List *temp = list;
  if (temp == NULL)
    return -1;
  while (i != index) {
    temp = temp->next;
    i++;
  }
  return temp->data;
}
