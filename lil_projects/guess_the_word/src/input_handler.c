#include "input_handler.h"
#include "utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// Returns newly allocated string without '\n'.
// Returns NULL on error
// status_code describes the reason.

char *load_user_input(bool *full_sentence, int *status_code) {

  size_t buff_cap = INPUT_BUFF_DEF_SIZE;
  char *buff = malloc(buff_cap * sizeof(char));
  if (buff == NULL) {
    *status_code = MALLOC_FAILED;
    return NULL;
  }

  size_t buff_index = 0;

  int c = fgetc(stdin);

  while (1) {
    if (ferror(stdin) || feof(stdin)) {
      *status_code = INPUT_ERROR;
      free(buff);
      return NULL;
    }
    if (buff_index + 1 >= buff_cap) {
      buff_cap *= 2;
      if (safe_realloc((void **)&buff, buff_cap, sizeof(char)) == false) {
        *status_code = REALLOC_FAILED;
        free(buff);
        return NULL;
      };
    }

    if (c != '\n') {
      buff[buff_index++] = c;
    } else {

      break;
    }
    c = fgetc(stdin);
  }

  buff[buff_index++] = '\0';

  if (buff_index <= 1) *full_sentence = false;
  if (buff_index > 1) *full_sentence = true;

  if (buff_index <= 1) // jen '\0'
    *status_code = BLANK_INPUT;
  else
    *status_code = OK;

  return buff;
}
