#include "modes.h"

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
    memcpy(current_project.saved_waves[input_key-'1'], current_wave, 32);
  }
  if(open_state && input_key <= '8' && input_key >= '1'){
    open_state = 0;
    memcpy(current_wave, current_project.saved_waves[input_key-'1'], 32);
  }
  for(int i=0; i<32; ++i){
    mvaddch(current_wave[i]+9, 2*i+1, '#');
    mvaddch(current_wave[i]+9, 2*i+2, '#');
  }
  mvaddch(currentY+1, 2*currentX+1, '@');
  mvaddch(currentY+1, 2*currentX+2, '@');
  if(save_state) mvaddstr(20, 0, "press numbere 1-8 to save\n");
  if(open_state) mvaddstr(20, 0, "press numbere 1-8 to open\n");
  return 0;
}
