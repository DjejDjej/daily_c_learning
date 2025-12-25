#ifndef GUESS_UTILS_H
#define GUESS_UTILS_H
#include <stdbool.h>
#include <stdio.h>

bool safe_realloc(void **ptr, size_t new_count, size_t elem_size);

#endif
