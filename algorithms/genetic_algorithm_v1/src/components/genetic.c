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

int find(int length, int target, int array[length]) {
  for (int i = 0; i < length; length++)
    if (target == array[i])
      return 1;
  return 0;
}

Solution *createSolution(int target, List *queue, int queueLength) {
  int sum = 0;
  int count = 0;
  int chosen_indexes[count];
  int random_index;
  // max retry is 3. retry excuse: we can't always trust randominess I guess...
  int retry_count = 0;
  Solution *solution = calloc(1, sizeof(Solution));
  if (solution == NULL)
    exit(-1);
  while (sum < target) {
    if (retry_count > 3)
      break;
    random_index = rand() % queueLength;
    int selectedValue = find_data_by_index(queue, random_index);
    if (find(count, random_index, chosen_indexes)) {
    } else if (sum + selectedValue > target)
      retry_count++;
    else {
      count++;
      sum += selectedValue;
      int *temp = realloc(solution->selected, sizeof(int) * count);
      if (temp == NULL) {
        free(solution->selected);
        free(solution);
        exit(-1);
      }
      chosen_indexes[count - 1] = random_index;
      solution->selected = temp;
      *(solution->selected + count - 1) = selectedValue;
    }
  }
  solution->selected_length = count;
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
        temp2 = temp1;
      temp1 = temp1->next;
      counter++;
    }
  } else {
    while (counter < index2) {
      if (counter == index1)
        temp1 = temp2;
      temp2 = temp2->next;
      counter++;
    }
  };

  Solution parent1 = temp1->solution;
  Solution parent2 = temp2->solution;
  int count = 0, sum = 0, i, j, retry_count = 0;
  Solution *newSolution = calloc(1, sizeof(Solution));
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
        break;
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
      sum += selectedValue;
      newSolution->selected_length = count;
      newSolution->selected = temp;
      *(newSolution->selected + count - 1) = selectedValue;
    }
  }

  newSolution->selected_length = count;
  newSolution->fitness = fitness(target, count, newSolution->selected);
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
  if (set->selected_length == 0 || set_length == 0)
    return;
  int sum = 0;
  for (int i = 0; i < set->selected_length; i++)
    sum += set->selected[i];
  int changedBit = rand() % set->selected_length;
  for (int i = 0; i < set_length * 2; i++) {
    int value = find_data_by_index(list, rand() % set_length);
    if (sum - set->selected[changedBit] + value <= target) {
      set->selected[changedBit] = value;
      set->fitness = fitness(target, set->selected_length, set->selected);
      break;
    }
  }
}

// generate
void generate_new_solutions(List *list, SolutionSet **set, int *set_length,
                            int target, SolutionSet **Before_Middle,
                            float mutation_ratio, int generation_size) {
  const int constantHalfSetLength = (*set_length);
  int list_length = 0;
  for (List *temp = list; temp != NULL; temp = temp->next)
    list_length++;
  while (*set_length < generation_size) {
    int parent1_index = rand() % constantHalfSetLength;
    int parent2_index;
    do {
      parent2_index = rand() % constantHalfSetLength;
    } while (parent1_index == parent2_index);
    Solution *pmx1 = pmx_set(parent1_index, parent2_index, *set, target);
    Solution *pmx2 = pmx_set(parent2_index, parent1_index, *set, target);
    int mutation_random = rand() % 100 + 1;
    if (mutation_random <= mutation_ratio * 100)
      mutation(list, pmx1, list_length, target);
    push_solution_set_with_order(set, pmx1, set_length, Before_Middle);
    free(pmx1);
    if (*set_length < generation_size) {
      mutation_random = rand() % 100 + 1;
      if (mutation_random <= mutation_ratio * 100)
        mutation(list, pmx2, list_length, target);
      push_solution_set_with_order(set, pmx2, set_length, Before_Middle);
    } else {
      free(pmx2->selected);
    }
    free(pmx2);
  }
}
