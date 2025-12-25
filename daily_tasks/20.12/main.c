#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum Return_code { OK = 0, NULL_INPUT = 1, BLANK_INPUT = 2, DELIM_NOT_FOUND = 3, MALLOC_FAILED = 4 };

typedef struct {
  int status;
  char *left;
  char *right;
} split_result;

split_result safe_split_once(const char *input, char delim) {
  char *_right;
  char *_left;
  split_result r = {0, NULL, NULL};
  int left_size = -1;
  int right_size = 0;

  if (input == NULL) {
    r.status = NULL_INPUT;
    return r;
  }

  int size = strlen(input);
  if (size == 0) {
    r.status = BLANK_INPUT;
    return r;
  }

  for (size_t i = 0; i < size; i++) {
    if (input[i] == delim) {
      left_size = i;
      break;
    }
  }

  if (left_size == -1) {
    r.status = DELIM_NOT_FOUND;
    return r;
  }

  right_size = (size - left_size - 1); // Is it good to have the calculation done once and than use it?

  _left = malloc(left_size + 1);
  _right = malloc(right_size + 1);

  if (_left == NULL || _right == NULL) {
    r.status = MALLOC_FAILED;
    return r;
  }

  strncpy(_left, input, left_size);
  _left[left_size] = '\0';
  r.left = _left;

  strncpy(_right, input + left_size + 1, right_size);
  _right[right_size] = '\0';
  r.right = _right;

  r.status = OK;

  return r;
}

int main() {
  char a[] = "aaa aaa";
  split_result r = safe_split_once(a, ' ');
  printf("%s||%s status: %d ", r.left, r.right, r.status);
  if (r.status == OK) {
    free(r.left);
    free(r.right);
  }
  return 0;
}
