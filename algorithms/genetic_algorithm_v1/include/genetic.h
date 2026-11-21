#ifndef GENETIC_H
#define GENETIC_H

#include "list.h"

typedef struct Solution {
  float fitness;
  int selected_length;
  int *selected; // Indices in the input list
} Solution;

typedef struct SolutionSet {
  struct Solution solution;
  struct SolutionSet *next;
} SolutionSet;

Solution *createSolution(int target, List *queue, int queueLength);
void push_solution_set_with_order(SolutionSet **set, Solution *new,
                                  int *setLength, SolutionSet **Before_Middle);
void set_selection(SolutionSet **set, int *setLength,
                   SolutionSet **BeforeMiddle);

void generate_new_solutions(List *list, SolutionSet **set, int *set_length,
                            int target, SolutionSet **Before_Middle,
                            float mutation_ratio, int generation_size);

#endif
