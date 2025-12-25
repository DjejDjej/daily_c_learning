#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 6

enum ret_code {
  EndOfFile = 1,
  FERR = 2,
  OVERFLOW = 3

};

int read_line(char *buffer, size_t buffer_length) {

  // Fgets fails for 2 reasons
  if (fgets(buffer, buffer_length, stdin) == NULL) {
    if (feof(stdin)) {
      fflush(stdin);
      return EndOfFile; /// EOF
    }
    if (ferror(stdin)) {
      fflush(stdin);
      return FERR; // Broken IO
    }
  }

  size_t str_buf_len = strlen(buffer);

  for (size_t i = 0; i < str_buf_len; i++) {

    if (buffer[i] == '\n') { // new line
      buffer[i] = '\0';
      return 0;
    }
  }

  return OVERFLOW;
}

int main(int argv, char **argc) {

  char *res = malloc(BUF_SIZE);
  if (!res) return 1; // Malloc failed
  int code = read_line(res, BUF_SIZE);
  if (code > 0) {
    printf("%d", code);
    free(res);
    return 1;
  }
  printf("%s\n", res);
  free(res);

  return 0;
}
