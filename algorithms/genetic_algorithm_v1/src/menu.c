#include "menu.h"
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

  printf("answer(DEFAULT 0): ");

  int choice = getchar() - '0';

  if (choice >= 0 && choice < total_options)
    return choice;

  return 0;
}
