#include "cmd.h"

static int is_valid(char c){
  if(c >= 'a' && c <= 'z') return 1;
  if(c >= '0' && c <= '9') return 1;
  if(c == ' ') return 1;
  return 0;
}

static char current_cmd[256];
static int cmd_top;

void handle_commands(void){
  int maxy = getmaxy(stdscr);
  int key = 0;
  while(key != '\n'){
    mvprintw(maxy-1, 0, "> %s", current_cmd);
    refresh();
    key = getch();
    if(is_valid(key)){
      current_cmd[cmd_top++] = key;
      current_cmd[cmd_top] = 0;
    }
  }

  // get the first word
  char first_word[256];
  int t = 0;
  memcpy(first_word, current_cmd, 256);
  while(first_word[t] != ' ') t++;
  first_word[t] = 0;

  #define IF_MATCH(...)\
  if(!strcmp(first_word, #__VA_ARGS__))

  IF_MATCH(tempo){
    int n = 0;
    if(sscanf(&current_cmd[t], "%d", &n) == 1)
      current_project.tempo = n;
  }
  // will add more

  t = 0;
  cmd_top = 0;
  current_cmd[0] = 0;
}
