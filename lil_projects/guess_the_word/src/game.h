#ifndef GUESS_GAME_H
#define GUESS_GAME_H
#include "file_handler.h"
#include "file_handler.h"
#include <stdio.h>
#define NAME_MAX_LEN 20
#define MAX_PLAYERS 5

struct player {

  char *name;
  int points;
};

struct game {
  struct player *players;
  size_t num_of_players;
  char *obfs_sentence;
  struct sentence *sent;
  int wheel_value;
};

int init_game();
int spin_wheel(int wheel_values[], size_t count);
int get_rnd_sentence_index(struct sentences *sent);
char *obfuscate_sentence(const struct sentences *sent, size_t index) ;
int char_guessed(int c, char *obfs_sentence, const struct sentences *sent, size_t index) ;
#endif
