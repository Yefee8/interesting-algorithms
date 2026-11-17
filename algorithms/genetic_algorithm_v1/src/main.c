#include <stdio.h>

#include "list.h"
#include "menu.h"
#include "test_cases.h"

int main(void) {
  List *Queue = NULL;
  int targetedValue;
  int choice = menu_controls();

  if (choice == 1)
    prepare_automatic_set(&Queue, &targetedValue);
  else {
    prepare_manuel_set(&Queue, &targetedValue);
  }

  // List *temp = Queue;
  // while (temp != NULL) {
  //   printf("\n%d", temp->data);
  //   temp = temp->next;
  // }

  free_list(Queue);

  return 0;
}
