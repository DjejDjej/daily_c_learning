#include "game.h"
#include "file_handler.h"
#include "input_handler.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Num of players, player names,

int init_game() {

  struct player players[MAX_PLAYERS];
  int status;
  bool fullf;

  char *input;
  int count_players = 1;
  while (1) {

    printf("how many players? max %d\n",MAX_PLAYERS );
    input = load_user_input(&fullf, &status);
    count_players = atoi(input);
    if (count_players > 0 && count_players <= 5) { break; }
  }

  if (count_players == 0) return -1;
  int index = 0;
  while (index < count_players) {
    printf("Whats players%d name?\n", index + 1);
    input = load_user_input(&fullf, &status);
    if (input == NULL) {
      printf("NULL");
      return -1;
    }
    if (input != NULL && strlen(input) < NAME_MAX_LEN) {
      players[index] = (struct player){input, 0};
      index++;
    } else {
      printf("Name cannot be longer than %d characters\n", NAME_MAX_LEN);
    }
  }


return 0;
  char *path = "sentences";
  char *mode = "r";
  struct file_handle *fh = open_file(path, mode, &status);
  struct sentences *sent = get_sentences_from_fh(fh, &status);
  int sentence_index = get_rnd_sentence_index(sent);
  char *obf_sentence = obfuscate_sentence(sent, sentence_index);
  size_t sent_size = strlen(obf_sentence);
  size_t guessed_count = 0;
  struct game g = {players, 2, obf_sentence, 0};
  // game loop
  return 0;
}

int game_loop(struct game *g) {
  // while (sent_size < guessed_count) {}
}

int render_game(struct game g) {

  printf("");
  return 0;
}

int rnd_in_range(int max, int mod) {
  srand(time(NULL) + mod);
  return rand() % max;
}

int spin_wheel(int wheel_values[], size_t count) {
  int wheel_index = rnd_in_range(count, 10);
  return wheel_values[wheel_index];
}

int char_guessed(int c, char *obfs_sentence, const struct sentences *sent, size_t index) {
  size_t s_size = strlen(sent->data[index]);
  size_t hitted_c = 0;
  if (obfs_sentence == NULL) return -1;
  if (c == ' ') return 0; // player cannot guess ' '
  for (size_t i = 0; i < s_size; i++) {
    if (sent->data[index][i] == c && obfs_sentence[i] != sent->data[index][i]) {
      obfs_sentence[i] = c;
      hitted_c++;
    }
  }
  return hitted_c;
}

char *obfuscate_sentence(const struct sentences *sent, size_t index) {
  size_t s_size = strlen(sent->data[index]);
  char *obf_sentence = malloc(s_size + 1);
  if (obf_sentence == NULL) return NULL;

  for (size_t i = 0; i < s_size; i++) {
    if (sent->data[index][i] == ' ') {
      obf_sentence[i] = ' ';
    } else {
      obf_sentence[i] = '-';
    }
  }
  obf_sentence[s_size] = '\0';
  return obf_sentence;
}

int get_rnd_sentence_index(struct sentences *sent) {

  int sent_index = rnd_in_range(sent->count, 30);

  return sent_index;
}
