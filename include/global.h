#ifndef GLOBAL_H
#define GLOBAL_H

#include <ncurses.h>
#include <sys/time.h>
#include "short_types.h"
#include <portaudio.h>

extern float dt;
extern int input_key;
extern struct timeval t1, t2;
extern i8 waves[16][32];

#endif
