#include "vector.h"
#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#define DEF_VEC_CAP 10

enum status_code {
  VEC_OK = 0,
  VEC_INVALID_ELEM_SIZE = 1,
  VEC_MALLOC_FAILED = 2,
  VEC_REALLOC_FAILED = 3,

};

int vec_init(struct vec *v, size_t elem_size) {
  v->capacity = DEF_VEC_CAP;
  v->count = 0;
  if (elem_size <= 0) { return VEC_INVALID_ELEM_SIZE; }
  v->elem_size = elem_size;

  void *data = malloc(v->capacity * v->elem_size);
  if (data == NULL) return VEC_MALLOC_FAILED;

  v->data = data;
  return 0;
}
void vec_free(struct vec *v) {
  free(v->data);
}
int vec_push(struct vec *v, void *elem) {
  if (v->capacity <= v->count) {
    v->capacity *= 2;

    void *tmp = realloc(v->data, v->capacity * v->elem_size);

    if (tmp == NULL) return VEC_REALLOC_FAILED;

    v->data = tmp;
  }
  void *base = v->data;
  size_t offset = v->count * v->elem_size;
  memcpy(base + offset, elem, v->elem_size);
  v->count++;
  return VEC_OK;
}
void *vec_get(struct vec *v, size_t index) {

  if (index >= v->count) return NULL;
  
  return (char *)v->data + index * v->elem_size;
}
size_t vec_count(const struct vec *v) {

  return v->count;
}
