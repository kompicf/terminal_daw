#include <string.h>
#include "config.h"
#include "global.h"
#include "render.h"

// could have made same as default.c
static char render[18][67] = {
  "+----------------------------------------------------------------+",
  "|                                                                |",
  "|                                                                |",
  "|                                                                |",
  "|                                                                |",
  "|                                                                |",
  "|                                                                |",
  "|                                                                |",
  "|                                                                |",
  "|                                                                |",
  "|                                                                |",
  "|                                                                |",
  "|                                                                |",
  "|                                                                |",
  "|                                                                |",
  "|                                                                |",
  "|                                                                |",
  "+----------------------------------------------------------------+",
};

i8 *current_wave;
i8 saved_waves[8][32];
static i8 currentX = 0;
static i8 currentY = 0;
// could have done bitmask
static i8 playing = 0;
static i8 save_state = 0;
static i8 open_state = 0;

int wavemaker_refresh(void){
  for(int i=0; i<18; ++i){
    mvaddstr(i, 0, render[i]);
  }
  if(input_key == keybinds.left && currentX != 0) currentX--;
  else if(input_key == keybinds.right && currentX != 31) currentX++;
  else if(input_key == keybinds.down && currentY != 15) currentY++;
  else if(input_key == keybinds.up && currentY != 0) currentY--;
  else if(input_key == keybinds.set_sample){
    current_wave[(size_t)currentX] = currentY-8;
    if(currentX != 31) currentX++;}
  else if(input_key == keybinds.play){
    playing = ~playing;
    if(playing) play_change(current_wave, 4, 0, 0);
    else play_change(current_wave, 0, 0, 0);}
  else if(input_key == keybinds.save_wave) save_state = ~save_state;
  else if(input_key == keybinds.load_wave) open_state = ~open_state;


  if(save_state && input_key <= '8' && input_key >= '1'){
    save_state = 0;
    memcpy(saved_waves[input_key-'1'], current_wave, 32);
  }
  if(open_state && input_key <= '8' && input_key >= '1'){
    open_state = 0;
    memcpy(current_wave, saved_waves[input_key-'1'], 32);
  }
  for(int i=0; i<32; ++i){
    mvaddch(current_wave[i]+9, 2*i+1, '#');
    mvaddch(current_wave[i]+9, 2*i+2, '#');
  }
  mvaddch(currentY+1, 2*currentX+1, '@');
  mvaddch(currentY+1, 2*currentX+2, '@');
  mvaddstr(20, 0,
    "use wasd or hjkl to move, enter to set sample,\n"
    "o to exit wavemaker\n"
    "f to save wave (f again to cancel)\n"
    "g to open wave (g again to cancel)\n"
    "space to preview wave (space again to stop)");
  if(save_state) mvaddstr(22, 0, "press numbere 1-8 to save\n");
  if(open_state) mvaddstr(23, 0, "press numbere 1-8 to open\n");
  return 0;
}
