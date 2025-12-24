#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define DEF_ARR_CAP 10
#define DEF_BUFF_CAP 10
#define DEF_WORDS_CAP 10

enum {
  FOUND_ELEMENT = -1,
  SPLIT_INPROG = -1,
  SPLIT_OK = 0,
  SPLIT_REALLOC_FAILED = 1,
  MALLOC_FAILED = 2,

};

struct word_count {
  char *word;
  size_t freq;
  size_t word_len;
};

struct words_count {
  struct word_count *words;
  size_t count;
  size_t status_code;
};

struct splited_arr {
  char **data;
  int status_code;
  size_t count;
};

int safe_realloc_generic(void **ptr, size_t new_count, size_t elem_size) {
  void *tmp = realloc(*ptr, new_count * elem_size);
  if (!tmp) return SPLIT_REALLOC_FAILED;
  *ptr = tmp;
  return SPLIT_OK;
}

int clean_up(char **data, size_t count) {

  for (size_t i = 0; i < count; i++) { free(data[i]); }
  free(data);
  return SPLIT_OK;
}

int clean_up_word_c(struct word_count *data, size_t count) {
  for (size_t i = 0; i < count; i++) { free(data[i].word); }
  free(data);
  return SPLIT_OK;
}

struct splited_arr split_into_arr(FILE *f) {
  struct splited_arr r = {NULL, SPLIT_INPROG, 0};

  size_t arr_cap = DEF_ARR_CAP;
  size_t arr_count = 0;
  size_t buff_cap = DEF_BUFF_CAP;

  char **arr = malloc(arr_cap * sizeof(char *));
  char *buff = malloc(buff_cap * sizeof(char));
  if (arr == NULL) {
    r.status_code = MALLOC_FAILED;
    free(buff);
    return r;
  }
  if (buff == NULL) {
    r.status_code = MALLOC_FAILED;
    free(arr);
    return r;
  }

  int ch = 0;
  int i = 0;
  ch = fgetc(f);
  while (1) {
    if (ch == EOF) break;
    if (arr_cap <= arr_count) {
      arr_cap *= 2;
      if (safe_realloc_generic((void **)&arr, arr_cap, sizeof(*arr)) == SPLIT_REALLOC_FAILED) {
        clean_up(arr, arr_count);
        free(buff);
        return r;
      }
    }
    if (buff_cap + 1 <= i) {
      buff_cap *= 2;
      if (safe_realloc_generic((void **)&buff, buff_cap, sizeof(*buff)) == SPLIT_REALLOC_FAILED) {
        clean_up(arr, arr_count);
        free(buff);
        return r;
      }
    }

    if (ch == ' ' || ch == '\n' || ch == '\t') {
      buff[i] = '\0';
      if (strlen(buff) > 0) {
        arr[arr_count] = malloc((i + 1) * sizeof(char));
        strcpy(arr[arr_count++], buff);
      }
      i = 0;
    } else {
      buff[i++] = ch;
    }
    ch = fgetc(f);
  }

  free(buff);

  r.data = arr;
  r.count = arr_count;
  r.status_code = SPLIT_OK;
  return r;
}

int word_exists(struct word_count *words, char *word, size_t size) {
  size_t word_len = strlen(word);
  for (size_t i = 0; i < size; i++) {
    if (words[i].word_len != word_len) continue;
    if (strcmp(words[i].word, word) == 0) return i;
  }

  return FOUND_ELEMENT;
}

struct words_count get_words_count(struct splited_arr split_arr) {

  size_t words_cap = DEF_WORDS_CAP;
  size_t words_count = 0;
  struct word_count *words = malloc(words_cap * sizeof(struct word_count));
  struct words_count ws = {NULL, 0, SPLIT_INPROG};

  if (split_arr.data == NULL) {
    free(words);
    ws.status_code = MALLOC_FAILED;
    return ws;
  }

  for (size_t i = 0; i < split_arr.count; i++) {
    int wi = word_exists(words, split_arr.data[i], words_count);

    if (words_count >= words_cap) {
      words_cap *= 2;
      if (safe_realloc_generic((void **)&words, words_cap, sizeof(*words)) == SPLIT_REALLOC_FAILED) {
        clean_up_word_c(words, words_count);
        free(words);
        ws.status_code = SPLIT_REALLOC_FAILED;
	return ws;
      };
    }

    if (wi == FOUND_ELEMENT) {

      size_t buff_len = strlen(split_arr.data[i]);
      char *buff = malloc(buff_len + 1);
      if (buff == NULL) {
        clean_up_word_c(words, words_count);
        ws.status_code = MALLOC_FAILED;
        free(words);
        return ws;
      }
      buff[buff_len] = '\0';

      buff = strcpy(buff, split_arr.data[i]);
      words[words_count++] = (struct word_count){.word = buff, .freq = 1, .word_len = buff_len};

    } else {
      words[wi].freq++;
    }
  }
  ws.words = words;
  ws.count = words_count;
  ws.status_code = SPLIT_OK;
  return ws;
}
int main() {

  FILE *f = fopen("file", "r");
  struct splited_arr r = split_into_arr(f);
  struct words_count rr = get_words_count(r);
  for (size_t i = 0; i < r.count; i++) { free(r.data[i]); }
  free(r.data);

  for (size_t i = 0; i < rr.count; i++) {

    printf("%s used: %lu \n", rr.words[i].word, rr.words[i].freq);
    free(rr.words[i].word);
  }
  free(rr.words);
  return 0;
}
