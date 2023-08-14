#include "config.h"

struct Keybinds keybinds;

struct Keybinds default_keybinds = {
  .increase_octave = 'y',
  .decrease_octave = 't',
  .up = 'w',
  .down = 's',
  .left = 'a',
  .right = 'd',
  .save_wave = 'f',
  .load_wave = 'g',
  .set_sample = '\n',
  .play = ' ',
  .play_from_cursor = ' ',
  .insert_mode = 'i',
  .replace_mode = 'r',
  .open_wavemaker = '0',
  .open_keybind_editor = 'l',
  .quit = 'q'
};

int valid_key(char key){
  // letters are valid
  if(key <= 'z' && key >= 'a') return 1;
  // other valid characters:
  if(key == ' ' || key == '\n') return 1;
  return 0;
}

void load_keybinds(void){
  FILE*file = fopen("config.txt","rb");
  if(file == NULL){ keybinds = default_keybinds; return; }
  fread(&keybinds, sizeof keybinds, 1, file);
}

void save_keybinds(void){
  FILE*file = fopen("config.txt","wb");
  if(file == NULL){
    fprintf(stderr, "Couldn't open config.txt\n");
    return; }
  fwrite(&keybinds, sizeof keybinds, 1, file);
}

#define DO(...)\
  mvprintw(2, 2, #__VA_ARGS__":");\
  refresh();\
  while(!valid_key(key)){ key = getch(); }\
  keybinds.__VA_ARGS__ = key;\
  key = 0;

void keybind_editor(void){
  char key = 0;
  DO(increase_octave);
  DO(decrease_octave);
  DO(up);
  DO(down);
  DO(left);
  DO(right);
  DO(save_wave);
  DO(load_wave);
  DO(set_sample);
  DO(play);
  DO(play_from_cursor);
  DO(insert_mode);
  DO(replace_mode);
  DO(open_wavemaker);
  DO(open_keybind_editor);
  DO(quit);

  save_keybinds();
}
