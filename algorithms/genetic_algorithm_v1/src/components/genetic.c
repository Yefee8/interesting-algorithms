#include "genetic.h"
#include "list.h"
#include <stddef.h>
#include <stdlib.h>
#include <time.h>

float fitness(int target, int items_length, int items[items_length]) {
  int sum = 0;
  for (int i = 0; i < items_length; i++)
    sum += items[i];
  return (float)sum / target;
}

void push_solution_set_with_order(SolutionSet **set, Solution *new) {
  SolutionSet *temp = *set;
  SolutionSet *lastItem = NULL;
  SolutionSet *newItem =
      calloc(sizeof(SolutionSet) - sizeof(Solution) + sizeof(*new), 1);
  if (newItem == NULL)
    exit(-1);

  if (*set == NULL)
    *set = newItem;

  newItem->solution = *new;
  newItem->next = NULL;
  while (temp != NULL) {
    if (newItem->solution.fitness >= temp->solution.fitness) {
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

Solution *createSolution(int target, List *queue, int queueLength) {
  srand(time(NULL));
  int sum = 0;
  int count = 0;
  // max retry is 3. retry excuse: we can't always trust randominess I guess...
  int retry_count = 0;
  Solution *solution = calloc(sizeof(Solution), 1);

  while (sum < target) {
    if (retry_count > 3)
      break;
    int selectedValue = find_data_by_index(queue, rand() % queueLength);
    if (sum + selectedValue > target)
      retry_count++;
    else {
      count++;
      int *temp = realloc(solution->selected, sizeof(int) * count);
      if (temp == NULL) {
        free(solution->selected);
        free(solution);
        exit(-1);
      }
      solution->selected = temp;
      *(solution->selected + count - 1) = selectedValue;
    }
  }
  solution->fitness = fitness(target, count, solution->selected);
  return solution;
}
