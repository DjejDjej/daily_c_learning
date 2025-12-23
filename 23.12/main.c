#include "vector.h"
#include <stdio.h>
#include <stdlib.h>

int main() {

  struct vec v;
  vec_init(&v, sizeof(int));
  int num = 67;
  vec_push(&v, &num);
  void *statnumber = vec_get(&v, 0);
  if(statnumber != NULL) printf("%d\n", *(int *)statnumber);


  for (int i = 0; i < 50; i++) { vec_push(&v, &i); }

  for (int i = 0; i < v.count; i++) {

    void *number = vec_get(&v, i);
    printf("%d\n", *(int *)number);
  }

  if(statnumber != NULL) printf("%d\n", *(int *)statnumber);
  vec_free(&v);
  return 0;
}
