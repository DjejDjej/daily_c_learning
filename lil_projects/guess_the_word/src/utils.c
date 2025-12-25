#include <stdbool.h>
#include <stdlib.h>

bool safe_realloc(void **ptr, size_t new_count, size_t elem_size) {
  void *tmp = realloc(*ptr, new_count * elem_size);
  if (!tmp) return false;
  *ptr = tmp;
  return true;
}
