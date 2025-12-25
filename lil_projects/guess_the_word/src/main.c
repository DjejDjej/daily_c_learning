#include "input_handler.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
  char *input;
  int status;
  bool sentence;

  do {
    input = load_user_input(&sentence, &status);
    printf("input = %s\n", input);
    printf("status = %d\n", status);
  } while (input != NULL);

  return 0;
}
