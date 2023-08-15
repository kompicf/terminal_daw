#ifndef RENDER_H_1234
#define RENDER_H_1234

#include "global.h"

// wavemaker
extern i8 *current_wave;
extern int wavemaker_refresh(void);

// default
extern void render_default_refresh(void);
extern void render_default_init(void);
extern void play_all(void);

// cmd
extern void handle_commands(void);

// config
struct Keybinds{
  char increase_octave;
  char decrease_octave;

  // movement
  char up;
  char down;
  char left;
  char right;

  // wavemaker
  char save_wave;
  char load_wave;
  char set_sample;

  char play;
  char play_from_cursor;
  char insert_mode;
  char replace_mode;

  // modes
  char open_wavemaker;
  char open_keybind_editor;
  char open_command_line;
  char quit;
};
extern struct Keybinds keybinds;
extern void load_keybinds(void);
extern void save_keybinds(void);
extern void keybind_editor(void);

#endif

