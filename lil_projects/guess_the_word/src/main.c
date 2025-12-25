#include "file_handler.h"
#include "input_handler.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
  char *input;
  int status;
  bool sentence;

  char *path = "sentences";
  char *mode = "r";
  struct file_handle *fh = open_file(path, mode, &status);
  struct sentences *sent = get_sentences_from_fh(fh, &status);  



  // do {
  //   input = load_user_input(&sentence, &status);
  //
  //   printf("input = %s\n", input);
  //   printf("status = %d\n", status);
  // } while (input != NULL);
  //
  //

free(fh);
  return 0;
}
