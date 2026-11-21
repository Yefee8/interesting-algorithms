#ifndef GENETIC_H
#define GENETIC_H

#include "list.h"

typedef struct Solution {
  float fitness;
  int selected_length;
  int *selected;
} Solution;

typedef struct SolutionSet {
  struct Solution solution;
  struct SolutionSet *next;
} SolutionSet;

void set_selection(SolutionSet **set, int *setLength,
                   SolutionSet **BeforeMiddle);

void generate_new_solutions(List *list, SolutionSet **set, int *set_length,
                            int target, SolutionSet **Before_Middle,
                            float mutation_ratio);

#endif
