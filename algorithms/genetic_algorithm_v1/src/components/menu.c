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
    if (scanf(" %d", &scanned_value) != 1)
      return;
    if (scanned_value > 0)
      push_with_order(set, scanned_value);
  } while (scanned_value != 0);
  printf("\nTargeted Value: ");
  if (scanf(" %d", targeted_value) != 1)
    *targeted_value = 0;
}

float ask_mutation_ratio(void) {
  float ratio;
  printf("\nMutation ratio (0-1): ");
  if (scanf(" %f", &ratio) != 1 || ratio < 0 || ratio > 1)
    return -1;
  return ratio;
}

int ask_generation_count(void) {
  int count;
  printf("\nNumber of generations (0 to stop): ");
  if (scanf(" %d", &count) != 1)
    return 0;
  return count;
}

int ask_generation_size(void) {
  int size;
  printf("\nGeneration size (minimum 4): ");
  if (scanf(" %d", &size) != 1 || size < 4)
    return 0;
  return size;
}
