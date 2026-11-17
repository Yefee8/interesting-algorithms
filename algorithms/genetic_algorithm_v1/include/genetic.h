#ifndef GENETIC_H
#define GENETIC_H

typedef struct Solution {
  float fitness;
  int selected_length;
  int *selected;
} Solution;

typedef struct SolutionSet {
  struct Solution solution;
  struct SolutionSet *next;
} SolutionSet;

#endif
