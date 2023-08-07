#ifndef RENDER_H_1234
#define RENDER_H_1234

#include <ncurses.h>
#include "global.h"

extern i8 saved_waves[8][32];
extern i8 *current_wave;
extern int wavemaker_refresh(void);
extern void render_default_refresh(void);
extern void render_default_init(void);
extern void play_all(void);

#endif
