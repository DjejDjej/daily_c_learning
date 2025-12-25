einclude <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEF_LINES_COUNT 10

enum status_code {
  OK = 0,
  REALLOC_FAILED = 1,

};

struct line_view {
  const char *ptr;
  size_t len;
};

struct line_views {
  struct line_view *lines;
  size_t count;
};

int safe_realloc(struct line_view **plines, size_t capacity) {
  void *tmp = realloc(*plines, capacity * sizeof(struct line_view));
  if (tmp == NULL) return REALLOC_FAILED;

  *plines = tmp;
  return OK;
}

struct line_views split_lines_view(const char *buff) {
  struct line_views r = {NULL, 0};
  if (!buff) return r;
  size_t buff_len = strlen(buff);
  size_t line_size = 0;
  size_t lines_count = 0;
  size_t plines_capacity = DEF_LINES_COUNT;

  struct line_view *plines = malloc(plines_capacity * sizeof(struct line_view));

  const char *curr = buff;
  for (size_t i = 0; i <= buff_len; i++) {

    if (lines_count >= plines_capacity) {
      plines_capacity *= 2;
      if (safe_realloc(&plines, plines_capacity)) {
        free(plines);
        return r;
      };
    }

    if (buff[i] == '\n' || i == buff_len) {
      const char *line_start = curr - line_size;
      plines[lines_count].ptr = line_start;
      plines[lines_count].len = line_size;
      line_size = 0;
      lines_count++;
    } else {
      line_size++;
    }

    curr++;
  }
  r.lines = plines;
  r.count = lines_count;
  return r;
}
int main() {
  char *buff =
      "buffer ahoj cau\n jak se dnes mas brasko ja mam slipy ruzove a ty mas doufam cerne\n pokud ne tak se zabij LOL";
  struct line_views r = split_lines_view(buff);
  for (size_t i = 0; i < r.count; i++) { printf("[%lu] %.*s\n", i, (int)r.lines[i].len, r.lines[i].ptr); }
  free(r.lines);
  return 0;
}
