#include "list.h"
#include <stdlib.h>

void push_with_order(List **activeQueue, int data) {
  List *newItem = calloc(1, sizeof(List));

  if (newItem == NULL)
    return;

  newItem->data = data;
  newItem->next = NULL;

  if (*activeQueue == NULL) {
    *activeQueue = newItem;
    return;
  }

  List *temp = *activeQueue;
  List *lastItem = NULL;

  while (temp != NULL) {
    if (newItem->data >= temp->data) {
      newItem->next = temp;

      if (lastItem != NULL)
        lastItem->next = newItem;
      else
        *activeQueue = newItem;

      return;
    }

    lastItem = temp;
    temp = temp->next;
  }

  lastItem->next = newItem;
}

void free_list(List *head) {
  while (head != NULL) {
    List *next = head->next;
    free(head);
    head = next;
  }
}
