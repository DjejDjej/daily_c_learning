#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define CODE_SIZE 10
#define CONT_DEF_S 10

enum status_codes {
  UNINITIALIZED = -1,
  OK = 0,
  REALLOC_FAILED = 1,
  MALLOC_FAILED = 2,
  EOF_ERROR = 3,
  IO_ERROR = 4
};

typedef struct {
  int status_code;
  char *content;
  size_t content_size;

} line;

int safeRealloc(line *l, int size) {
  void *tmp = realloc(l->content, size);
  if (tmp == NULL) {

    return REALLOC_FAILED;

  } else {
    l->content = tmp;
  }
  return OK;
}


line readline_malloc() { // Memory is owned by function.
  char *content = malloc(CONT_DEF_S);
  int buff;
  int i = 0;
  line l = {UNINITIALIZED, content, CONT_DEF_S};

  if (content == NULL) {
    l.status_code = MALLOC_FAILED;
    return l;
  }
  buff = getchar();
  if (buff == EOF) {
    if (feof(stdin)) l.status_code = EOF_ERROR;
    if (ferror(stdin)) l.status_code = IO_ERROR;
    return l;
  }

  while (buff != '\n' && buff != EOF) {

    if (i >= l.content_size) {
      l.content_size *= 2;

      if (safeRealloc(&l, l.content_size) == 1) {
        l.status_code = REALLOC_FAILED; 
        return l;
      }
    }

    l.content[i++] = buff;
    buff = getchar();
  }

  if (i >= l.content_size + 1) {

    if (safeRealloc(&l, l.content_size + 1) == 1) {
      l.status_code = REALLOC_FAILED; 
      return l;
    }
  }

  l.content[i++] = '\0';
  l.status_code = OK;
  return l;
}

int main() {

  line out = readline_malloc();
  printf("content: %s, status code: %d\n", out.content, out.status_code);
  free(out.content);
  return OK;
}
