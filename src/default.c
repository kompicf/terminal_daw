#include "modes.h"
#define CX cursorx[cursorchannel]
#define CY cursory[cursorchannel]
#define PITCH_B 4
#define WAVE_B 2
#define VOLUME_B 1

static i8 cursorx[4] = {0};
static i8 cursory[4] = {0};
static i8 cursorchannel = 0;

static char frame[2][42] = {
  "+=========+=========+=========+=========+",
  "|         |         |         |         |",
};

// y, channel, x[note, wave, volume, cmd]
static enum {insert, replace, normal} mode;

static i8 char_to_int(char x){
  for(int i=0; i<12; ++i){
    if(keybinds.keyboard[i] == x) return i;
  }
  return -1;
}

char *note_names[13] = {
  "A","A#","H","C","C#","D","D#","E","F","F#","G","G#"," "
};

static i8 global_octave = 0;

void render_default_init(void){
  mode = normal;
  current_project.tempo = 100;
}

void render_default_refresh(void){

  // draw frame and data
  mvaddstr(0, 1, frame[0]);
  for(int i=0; i<32; ++i){
    mvaddstr(i+1, 1, frame[1]);
    if(i%4 == 0) mvaddch(i+1, 0, '*');
    else if(i%2 == 0) mvaddch(i+1, 0, '-');

    for(int j=0; j<4; ++j){
      // pitch
      if(current_project.data[i][j].state & PITCH_B){
        mvaddstr(i+1, j*10+2, note_names[((int)current_project.data[i][j].pitch+48)%12]);
        mvaddch(i+1, j*10+4, (char)((current_project.data[i][j].pitch)/12+4) + '0');
      }
      else mvaddstr(i+1, j*10+2, "---");
      // wave
      if(current_project.data[i][j].state & WAVE_B)
        mvaddch(i+1, j*10+7, current_project.data[i][j].wave+'0');
      else mvaddch(i+1, j*10+7, '-');
      // volume
      if(current_project.data[i][j].state & VOLUME_B)
        mvaddch(i+1, j*10+9, current_project.data[i][j].volume+'0');
      else mvaddch(i+1, j*10+9, '-');
    }
  }
  mvaddstr(33, 1, frame[0]);

  switch(mode){
  case normal:
    // input
    if(input_key == keybinds.up && CY > 0) CY--;
    else if(input_key == keybinds.left && CX > 0) CX--;
    else if(input_key == keybinds.down && CY < 31) CY++;
    else if(input_key == keybinds.right && CX < 2) CX++;
    else if(input_key == keybinds.insert_mode) mode = insert;
    else if(input_key == keybinds.replace_mode) mode = replace;
    else if(input_key == keybinds.increase_octave && global_octave < 4) global_octave++;
    else if(input_key == keybinds.decrease_octave && global_octave > -4) global_octave--;
    else if(input_key == keybinds.play) play_all();
    else if(input_key >= '1' && input_key <= '4') cursorchannel = input_key - '1';
    // render
    switch(CX){
      case 0:
        mvaddch(CY+1, 5+cursorchannel*10, '<');
        mvaddch(CY+1, 6+cursorchannel*10, ':');
        break;
      case 1:
        mvaddch(CY+1, 5+cursorchannel*10, ':');
        mvaddch(CY+1, 6+cursorchannel*10, '>');
        break;
      case 2:
        mvaddch(CY+1, 10+cursorchannel*10, '*');
        break;
      default: break;
    }
    break;

  case replace:
    if(input_key == keybinds.replace_mode) mode = normal;
    goto normalreplace;
  case insert:
    if(input_key == keybinds.insert_mode){
      mode = normal;
      break;
    }
  normalreplace:
    switch(CX){
      case 0: // pitch
        mvaddch(CY+1, 5+cursorchannel*10, '<');
        mvaddch(CY+1, 6+cursorchannel*10, '=');
        if(input_key == keybinds.play){
          current_project.data[CY][cursorchannel].state &= ~PITCH_B;
          if(CY < 31 && mode == insert) CY++;
          if(mode == replace) mode = normal;
        }
        else if(input_key == keybinds.decrease_octave && current_project.data[CY][cursorchannel].pitch > -48){
          current_project.data[CY][cursorchannel].pitch -= 12;
          current_project.data[CY][cursorchannel].state |= PITCH_B;
          if(mode == replace) mode = normal;
        }
        else if(input_key == keybinds.increase_octave && current_project.data[CY][cursorchannel].pitch < 48){
          current_project.data[CY][cursorchannel].pitch += 12;
          current_project.data[CY][cursorchannel].state |= PITCH_B;
          if(mode == replace) mode = normal;
        }
        else if(char_to_int(input_key) != -1){
          current_project.data[CY][cursorchannel].pitch = char_to_int(input_key) + global_octave*12;
          current_project.data[CY][cursorchannel].state |= PITCH_B;
          if(CY < 31 && mode == insert) CY++;
          if(mode == replace) mode = normal;
        }
        break;

      case 1: // wave
        mvaddch(CY+1, 5+cursorchannel*10, '=');
        mvaddch(CY+1, 6+cursorchannel*10, '>');
        if(input_key == keybinds.play){
          current_project.data[CY][cursorchannel].state &= ~WAVE_B;
          mode = normal;
        }
      
        else if(input_key >= '1' && input_key <= '8'){
          current_project.data[CY][cursorchannel].wave = input_key - '0';
          current_project.data[CY][cursorchannel].state |= WAVE_B;
          mode = normal;
        }
        break;

      case 2: // volume
        if(input_key == keybinds.play){
          current_project.data[CY][cursorchannel].state &= ~VOLUME_B;
          mode = normal;
        }
        else if(input_key >= '0' && input_key <= '8'){
          current_project.data[CY][cursorchannel].volume = input_key - '0';
          current_project.data[CY][cursorchannel].state |= VOLUME_B;
          mode = normal;
        }
        mvaddch(CY+1, 10+cursorchannel*10, '=');
        break;
      default: break;
    }
    
    break;
  default: break;
  }
}

void play_all(void){
  int pitch[4] = {0};
  int volume[4] = {0};
  i8 *wave[4];
  for(int i=0; i<4; ++i) wave[i] = current_project.saved_waves[0];
  for(int i=0; i<32; ++i){
    for(int j=0; j<4; ++j){
      // apply changes only if they happen
      if(current_project.data[i][j].state & PITCH_B) pitch[j] = current_project.data[i][j].pitch;
      if(current_project.data[i][j].state & WAVE_B) wave[j] = current_project.saved_waves[current_project.data[i][j].wave - 1];
      else wave[j] = NULL;
      if(current_project.data[i][j].state & VOLUME_B) volume[j] = current_project.data[i][j].volume;
      play_change(wave[j], volume[j], pitch[j], j);
    }
    mvaddstr(i+1, 43, "<-");
    if(getch() == keybinds.play) goto end;
    refresh();
    Pa_Sleep(6000/current_project.tempo); // Pa_Sleep is in ms, tempo is in bpm
  }

end:
  // mute
  for(int j=0; j<4; ++j)
    play_change(NULL, 0, 0, j);
  return;
}
