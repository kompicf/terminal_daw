#ifndef PLAY_H_15784975
#define PLAY_H_15784975

#include <portaudio.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <limits.h>
#include "short_types.h"

extern void play_init(void);
extern void play_stop(void);
extern void play_change(i8 wave_in[32], int amplitude_in, int pitch_in, i8 channel);

#endif
