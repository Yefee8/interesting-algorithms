#include "genetic.h"
#include "list.h"
#include <stddef.h>
#include <stdlib.h>
#include <time.h>

float fitness(int target, List *list, int items_length, int items[items_length]) {
  int sum = 0;
  for (int i = 0; i < items_length; i++)
    sum += find_data_by_index(list, items[i]);
  return (float)sum / target;
}

int find(int length, int target, int array[length]) {
  for (int i = 0; i < length; i++)
    if (target == array[i])
      return 1;
  return 0;
}

Solution *createSolution(int target, List *queue, int queueLength) {
  int sum = 0;
  int count = 0;
  int random_index;
  int checked = 0;
  // max retry is 3. retry excuse: we can't always trust randominess I guess...
  int retry_count = 0;
  Solution *solution = calloc(1, sizeof(Solution));
  if (solution == NULL)
    exit(-1);
  int *indexes = malloc(sizeof(int) * queueLength);
  if (indexes == NULL) {
    free(solution);
    exit(-1);
  }
  for (int i = 0; i < queueLength; i++)
    indexes[i] = i;
  for (int i = queueLength - 1; i > 0; i--) {
    int index = rand() % (i + 1);
    int temp = indexes[i];
    indexes[i] = indexes[index];
    indexes[index] = temp;
  }
  while (sum < target && checked < queueLength) {
    if (retry_count > 3)
      break;
    random_index = indexes[checked++];
    int selectedValue = find_data_by_index(queue, random_index);
    if (sum + selectedValue > target)
      retry_count++;
    else {
      count++;
      sum += selectedValue;
      int *temp = realloc(solution->selected, sizeof(int) * count);
      if (temp == NULL) {
        free(indexes);
        free(solution->selected);
        free(solution);
        exit(-1);
      }
      solution->selected = temp;
      *(solution->selected + count - 1) = random_index;
    }
  }
  free(indexes);
  solution->selected_length = count;
  solution->fitness = fitness(target, queue, count, solution->selected);
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
Solution *pmx_set(int index1, int index2, SolutionSet *set, int target,
                  List *list) {
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
        sum += find_data_by_index(list, parent1.selected[i]);
        *(newSolution->selected + count - 1) = parent1.selected[i];
        break;
      }

  int checked = 0;
  int start_index = parent1.selected_length > 0 ? rand() % parent1.selected_length : 0;
  while (sum < target && checked < parent1.selected_length) {
    if (retry_count > 3)
      break;
    int selectedIndex = parent1.selected[(start_index + checked++) % parent1.selected_length];
    int selectedValue = find_data_by_index(list, selectedIndex);
    if (find(count, selectedIndex, newSolution->selected))
      continue;
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
      *(newSolution->selected + count - 1) = selectedIndex;
    }
  }

  newSolution->selected_length = count;
  newSolution->fitness = fitness(target, list, count, newSolution->selected);
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
    sum += find_data_by_index(list, set->selected[i]);
  int changedBit = rand() % set->selected_length;
  for (int i = 0; i < set_length * 2; i++) {
    int index = rand() % set_length;
    int value = find_data_by_index(list, index);
    int oldValue = find_data_by_index(list, set->selected[changedBit]);
    if (index != set->selected[changedBit] &&
        find(set->selected_length, index, set->selected))
      continue;
    if (sum - oldValue + value <= target) {
      set->selected[changedBit] = index;
      set->fitness = fitness(target, list, set->selected_length, set->selected);
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
    Solution *pmx1 = pmx_set(parent1_index, parent2_index, *set, target, list);
    Solution *pmx2 = pmx_set(parent2_index, parent1_index, *set, target, list);
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
