#include <stdio.h>

#include "list.h"
#include "menu.h"
#include "test_cases.h"

int main(void) {
  List *Queue = NULL;

  int choice = menu_controls();

  printf("choice: %d", choice);

  free_list(Queue);

  return 0;
}
