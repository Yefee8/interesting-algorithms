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

Solution *createSolution(int target, List *queue, int queueLength) {
  srand(time(NULL));
  int sum = 0;
  int count = 0;
  // max retry is 3. retry excuse: we can't always trust randominess I guess...
  int retry_count = 0;
  Solution *solution = calloc(sizeof(Solution), 1);
  if (solution == NULL)
    exit(-1);
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

void set_selection(SolutionSet **set, int *setLength,
                   SolutionSet **BeforeMiddle) {
  if (*setLength == 1) {
    free((*set)->solution.selected);
    free(*set);
    exit(0);
  }

  SolutionSet *middle = (*BeforeMiddle)->next;

  while (middle != NULL) {
    SolutionSet *temp = (middle)->next;
    free((middle)->solution.selected);
    free((middle));
    middle = temp;
    (*setLength)--;
  }

  (*BeforeMiddle)->next = NULL;
}

// pmx crossover(not exactly i know...) - ONLY RETURNS 1 CHILD!
Solution *pmx_set(int index1, int index2, SolutionSet *set, int target) {
  int counter = 0;
  SolutionSet *temp1 = set;
  SolutionSet *temp2 = set;

  // select the two of the solutions
  if (index1 > index2) {
    while (counter < index1) {
      if (counter == index2)
        *temp2 = *temp1;
      temp1 = temp1->next;
      counter++;
    }
  } else {
    while (counter < index2) {
      if (counter == index1)
        *temp1 = *temp2;
      temp2 = temp2->next;
      counter++;
    }
  };

  Solution parent1 = temp1->solution;
  Solution parent2 = temp2->solution;
  int count = 0, sum = 0, i, j, retry_count = 0;
  Solution *newSolution = calloc(sizeof(Solution), 1);
  if (newSolution == NULL)
    exit(-1);

  // select the same items
  for (i = 0; i < parent1.selected_length; i++)
    for (j = 0; j < parent2.selected_length; j++)
      if (parent1.selected[i] == parent2.selected[j]) {
        count++;
        int *temp = realloc(newSolution->selected, sizeof(int) * count);
        if (temp == NULL) {
          free(newSolution->selected);
          free(newSolution);
          exit(-1);
        }
        newSolution->selected = temp;
        sum += parent1.selected[i];
        *(newSolution->selected + count - 1) = parent1.selected[i];
      }

  while (sum < target) {
    if (retry_count > 3)
      break;
    int selectedValue = parent1.selected[rand() % parent1.selected_length];
    if (sum + selectedValue > target)
      retry_count++;
    else {
      count++;
      int *temp = realloc(newSolution->selected, sizeof(int) * count);
      if (temp == NULL) {
        free(newSolution->selected);
        free(newSolution);
        exit(-1);
      }
      newSolution->selected_length = count;
      newSolution->selected = temp;
      *(newSolution->selected + count - 1) = selectedValue;
    }
  }

  return newSolution;
}

// If we had prev in SolutionSet (double way linked list) it would be a O(1)
// solution, but making/understanding this made me better.
void update_before_middle(int setLength, int isBeforeOrEqual, SolutionSet **set,
                          SolutionSet **Before_Middle) {
  if (setLength == 2) {
    *Before_Middle = *set;
  } else if (isBeforeOrEqual == 0 && (setLength % 2 == 0)) {
    *Before_Middle = (*Before_Middle)->next;
  } else if (isBeforeOrEqual == 1 && (setLength % 2 == 1)) {
    int targetIndex = (setLength / 2) - 1;
    SolutionSet *temp = *set;
    for (int i = 0; i < targetIndex; i++) {
      temp = temp->next;
    }
    *Before_Middle = temp;
  }
}

void push_solution_set_with_order(SolutionSet **set, Solution *new,
                                  int *setLength, SolutionSet **Before_Middle) {
  SolutionSet *temp = *set;
  SolutionSet *lastItem = NULL;
  SolutionSet *newItem =
      calloc(1, sizeof(SolutionSet) - sizeof(Solution) + sizeof(*new));

  if (newItem == NULL)
    exit(-1);
  (*setLength)++;
  newItem->solution = *new;
  newItem->next = NULL;

  if (*set == NULL) {
    *Before_Middle = newItem;
    *set = newItem;
    return;
  }

  int isBeforeOrEqual =
      newItem->solution.fitness >= (*Before_Middle)->solution.fitness ? 1 : 0;
  if (isBeforeOrEqual == 0)
    temp = *Before_Middle;
  while (temp != NULL) {
    if (newItem->solution.fitness >= temp->solution.fitness) {
      newItem->next = temp;
      if (lastItem != NULL)
        lastItem->next = newItem;
      else
        *set = newItem;
      update_before_middle(*setLength, isBeforeOrEqual, set, Before_Middle);
      return;
    }

    lastItem = temp;
    temp = temp->next;
  }

  lastItem->next = newItem;

  update_before_middle(*setLength, isBeforeOrEqual, set, Before_Middle);
}

// mutation
void mutation(List *list, Solution *set, int set_length, int target) {
  srand(time(NULL));
  int sum = 0;
  for (int i = 0; i < set->selected_length; i++)
    sum += set->selected[i];
  int changedBit = rand() % set->selected_length;
  Solution *temp = set;
  int random_number_from_set = find_data_by_index(list, rand() % set_length);
  while (sum - set->selected[changedBit] + random_number_from_set > target)
    random_number_from_set = find_data_by_index(list, rand() % set_length);

  temp->selected[changedBit] = random_number_from_set;
  *set = *temp;
  free(temp);
}

// generate
void generate_new_solutions(List *list, SolutionSet **set, int *set_length,
                            int target, SolutionSet **Before_Middle,
                            float mutation_ratio) {
  srand(time(NULL));
  const int constantHalfSetLength = (*set_length);
  int counter = 0;
  while (counter < constantHalfSetLength * 2) {
    counter++;
    int parent1_index = rand() % constantHalfSetLength;
    int parent2_index;
    do {
      parent2_index = rand() % constantHalfSetLength;
    } while (parent1_index == parent2_index);
    Solution *pmx1 = pmx_set(parent1_index, parent2_index, *set, target);
    Solution *pmx2 = pmx_set(parent2_index, parent1_index, *set, target);
    int mutation_random = rand() % 99 + 1;
    if ((int)(mutation_ratio * 100) == mutation_random)
      mutation(list, pmx1, *set_length, target);
    push_solution_set_with_order(set, pmx1, set_length, Before_Middle);
    mutation_random = rand() % 99 + 1;
    if ((int)(mutation_ratio * 100) == mutation_random)
      mutation(list, pmx2, *set_length, target);
    push_solution_set_with_order(set, pmx2, set_length, Before_Middle);
  }
}
