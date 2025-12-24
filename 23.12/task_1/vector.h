#ifndef cpu_h
#define cpu_h
#include <stddef.h>
struct vec {
  void *data;
  size_t elem_size;
  size_t count;
  size_t capacity;
};
int vec_init(struct vec *v, size_t elem_size);
void vec_free(struct vec *v);
void *vec_get(struct vec *v, size_t index);
int vec_push(struct vec *v, void *elem);
size_t vec_count(const struct vec *v);
#endif
