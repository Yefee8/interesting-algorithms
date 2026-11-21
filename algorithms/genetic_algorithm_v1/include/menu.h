#include <list.h>
#ifndef MENU_H
#define MENU_H

int menu_controls(void);
float ask_mutation_ratio(void);
int ask_generation_count(void);
int ask_generation_size(void);
void prepare_manuel_set(List **set, int *targeted_value);
#endif
