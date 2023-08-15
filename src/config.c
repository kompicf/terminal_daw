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
  .quit = 'q',
  .keyboard = "zsxcfvgbnjmk"
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
  for(int i=0; i<12; ++i){
    mvprintw(i+2, 30, "%3s: %c", note_names[i], keybinds.keyboard[i]);
  }
}


void keybind_editor(void){
  int cursory = 0;
  int cursorx = 0;
  int key = getch();

  mvaddstr(0, 0, "1 2 3 4 - left, right, down, up, 5 - change key, 6 - exit");
  while(key != '6'){
    key = getch();
    mvaddch(cursory+2, cursorx*29, '-');
    display_keybinds();

    switch(key){
      case '1':
        if(cursorx == 1){
          mvaddch(cursory+2, cursorx*29, ' ');
          cursorx = 0;
          cursory = 0;
        }
        break;
      case '2':
        if(cursorx == 0){
          mvaddch(cursory+2, cursorx*29, ' ');
          cursorx = 1;
          cursory = 0;
        }
        break;
      case '3':
        if(cursory < 11){
          mvaddch(cursory+2, cursorx*29, ' ');
          cursory++;
        } else if(cursory < 16 && cursorx == 0){
          mvaddch(cursory+2, cursorx*29, ' ');
          cursory++;
        }
        break;
      case '4':
        if(cursory > 0){
          mvaddch(cursory+2, cursorx*29, ' ');
          cursory--;
        }
        break;
      case '5':
        mvaddch(cursory+2, cursorx*29, '>');
        while(!valid_key(key)){ key = getch(); }
        if(cursorx == 0)
          ((char*)&keybinds)[cursory] = key;
        else
          keybinds.keyboard[cursory] = key;
        mvaddch(cursory+2, cursorx*29, '-');
        break;
      default: break;
    }
  }

  save_keybinds();
}
