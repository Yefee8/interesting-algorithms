#include <stdio.h>
#include <stdlib.h>

typedef struct List {
  int data;
  struct List *next;
} List;

int testCases[10][100] = {0};

void clear_screen() {
#ifdef _WIN32
  system("cls");
#else
  system("clear");
#endif
}

void push_with_order(List **activeQueue, int data) {
  List *newItem = calloc(sizeof(List), 1);

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

void push_test_cases(List **activeQueue, int opt) {
  int i = 0;
  while (testCases[opt][i] != 0) {
    push_with_order(activeQueue, testCases[opt][i]);
    i++;
  }
}

// +1 for '\0'
char menu_options[2][13] = {"Manuel-Input", "Auto-Input"};

void menu_controls(List **activeQueue) {
  clear_screen();
  printf("\nGENETIC ALGORITHM\n");

  int total_options = (int)(sizeof(menu_options) / sizeof(menu_options[0]));
  for (int i = 0; i < total_options; i++) {
    printf("%d: %s", i, menu_options[i]);
    if (i == 0)
      printf(" [DEFAULT]");
    printf("\n");
  }

  // just getting the first char, if you type 12345 choice will be 1, same goes
  // for the ASCII characters.
  printf("answer(DEFAULT 0): ");
  int choice = getchar() - (int)'0';

  if (0 <= choice && choice < total_options) {
    printf("you chose: %d\n", choice);
  } else
    printf("you chose: auto 0 \n");
}

int main(void) {
  List *Queue = NULL;
  menu_controls(&Queue);
  return 0;
}
