#ifndef GLOBAL_H
#define GLOBAL_H

#include <ncurses.h>
#include <sys/time.h>
#include <portaudio.h>
#include "short_types.h"
#include "play.h"

extern float dt;
extern int input_key;
extern struct timeval t1, t2;

struct project{
  struct{
    i8 pitch;
    i8 wave;
    i8 volume;
    i8 state;
  } data[32][4];
  i8 saved_waves[8][32];
  unsigned int tempo;
  char name[20];
};

extern struct project current_project;

#endif
