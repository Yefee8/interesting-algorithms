#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "genetic.h"
#include "list.h"
#include "menu.h"

void print_solutions(SolutionSet *set) {
  for (SolutionSet *temp = set; temp != NULL; temp = temp->next) {
    printf("Fitness: %.3f, selected:", temp->solution.fitness);
    for (int i = 0; i < temp->solution.selected_length; i++)
      printf(" %d", temp->solution.selected[i]);
    printf("\n");
  }
}

int main(void) {
  List *Queue = NULL;
  int targetedValue = 0;
  prepare_manuel_set(&Queue, &targetedValue);

  int queueLength = 0;
  for (List *temp = Queue; temp != NULL; temp = temp->next)
    queueLength++;
  if (queueLength == 0 || targetedValue <= 0) {
    printf("A non-empty set and a positive target are required.\n");
    free_set(Queue);
    return 1;
  }

  float mutation_ratio = ask_mutation_ratio();
  if (mutation_ratio < 0) {
    printf("Invalid mutation ratio.\n");
    free_set(Queue);
    return 1;
  }

  int generation_size = ask_generation_size();
  if (generation_size == 0) {
    printf("Invalid generation size.\n");
    free_set(Queue);
    return 1;
  }

  srand(time(NULL));
  SolutionSet *set = NULL;
  SolutionSet *Before_Middle = NULL;
  int setLength = 0;
  for (int i = 0; i < generation_size; i++) {
    Solution *solution = createSolution(targetedValue, Queue, queueLength);
    push_solution_set_with_order(&set, solution, &setLength, &Before_Middle);
    free(solution);
  }

  printf("\nInitial generation:\n");
  print_solutions(set);

  int generations;
  while ((generations = ask_generation_count()) != 0) {
    if (generations < 0) {
      printf("Enter a non-negative number.\n");
      continue;
    }
    for (int i = 0; i < generations; i++) {
      set_selection(&set, &setLength, &Before_Middle);
      generate_new_solutions(Queue, &set, &setLength, targetedValue,
                             &Before_Middle, mutation_ratio, generation_size);
    }
    print_solutions(set);
  }

  while (set != NULL) {
    SolutionSet *next = set->next;
    free(set->solution.selected);
    free(set);
    set = next;
  }

  free_set(Queue);
  return 0;
}
