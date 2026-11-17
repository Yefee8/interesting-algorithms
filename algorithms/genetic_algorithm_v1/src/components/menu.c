#include "menu.h"
#include "list.h"
#include <stdio.h>
#include <stdlib.h>

static void clear_screen(void) {
#ifdef _WIN32
  system("cls");
#else
  system("clear");
#endif
}

int menu_controls(void) {
  static const char *menu_options[] = {"Manuel-Input", "Auto-Input"};

  clear_screen();

  printf("\nGENETIC ALGORITHM\n");

  int total_options = sizeof(menu_options) / sizeof(menu_options[0]);

  for (int i = 0; i < total_options; i++) {
    printf("%d: %s", i, menu_options[i]);

    if (i == 0)
      printf(" [DEFAULT]");

    printf("\n");
  }

  printf("answer[DEFAULT 0]: ");

  int choice = getchar() - '0';

  if (choice >= 0 && choice < total_options)
    return choice;

  return 0;
}

void prepare_manuel_set(List **set, int *targeted_value) {
  int scanned_value = 0;
  do {
    printf("\nNumber that you want in the set(write 0 to stop): ");
    scanf(" %d", &scanned_value);
    if (scanned_value != 0)
      push_with_order(set, scanned_value);
  } while (scanned_value != 0);
  printf("\nTargeted Value: ");
  scanf(" %d", targeted_value);
}
