#include "modes.h"

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
  .open_wavemaker = 'o',
  .open_keybind_editor = 'l',
  .open_command_line = '/',
  .quit = 'q'
};

int valid_key(char key){
  // letters are valid
  if(key <= 'z' && key >= 'a') return 1;
  // other valid characters:
  if(key == ' ' || key == '\n' || key == '/') return 1;
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

#define PRINT(...)\
  if(keybinds.__VA_ARGS__ == '\n')\
    mvprintw(2+i, 2, #__VA_ARGS__": '\\n'");\
  else\
    mvprintw(2+i, 2, #__VA_ARGS__": '%c'", keybinds.__VA_ARGS__);\
  i++;

static void display_keybinds(void){
  int i = 0;
  PRINT(increase_octave);
  PRINT(decrease_octave);

  PRINT(up);
  PRINT(down);
  PRINT(left);
  PRINT(right);

  PRINT(save_wave);
  PRINT(load_wave);
  PRINT(set_sample);

  PRINT(play);
  PRINT(play_from_cursor);
  PRINT(insert_mode);
  PRINT(replace_mode);

  PRINT(open_wavemaker);
  PRINT(open_keybind_editor);
  PRINT(open_command_line);
  PRINT(quit);
}

void keybind_editor(void){
  int cursory = 0;
  int key = getch();

  mvaddstr(0, 0, "Use 2 to go up, 1 for down, 3 to change key, 4 to exit");
  while(key != '4'){
    key = getch();
    mvaddch(cursory+2, 0, '-');
    display_keybinds();

    switch(key){
      case '1':
        if(cursory < (int)sizeof(struct Keybinds)-1){
          mvaddch(cursory+2, 0, ' ');
          cursory++;
        }
        break;
      case '2':
        if(cursory > 0){
          mvaddch(cursory+2, 0, ' ');
          cursory--;
        }
        break;
      case '3':
        mvaddch(cursory+2, 0, '>');
        while(!valid_key(key)){ key = getch(); }
        ((char*)&keybinds)[cursory] = key;
        mvaddch(cursory+2, 0, '-');
        break;
      default: break;
    }
  }

  save_keybinds();
}
