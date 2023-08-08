#include "global.h"
#include "render.h"
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
struct data_t{
  i8 pitch;
  i8 wave;
  i8 volume;
  i8 state;
};
struct data_t data[32][4];
static enum {insert, replace, normal} mode;

static i8 char_to_int(char x){
  switch(x){
    case 'z': return 0;
    case 's': return 1;
    case 'x': return 2;
    case 'c': return 3;
    case 'f': return 4;
    case 'v': return 5;
    case 'g': return 6;
    case 'b': return 7;
    case 'n': return 8;
    case 'j': return 9;
    case 'm': return 10;
    case 'k': return 11;
    default: return -1;
  }
}

char *note_names[13] = {
  "A","A#","H","C","C#","D","D#","E","F","F#","G","G#"," "
};

static i8 global_octave = 0;

void render_default_init(void){
  mode = normal;
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
      if(data[i][j].state & PITCH_B){
        mvaddstr(i+1, j*10+2, note_names[((int)data[i][j].pitch+48)%12]);
        mvaddch(i+1, j*10+4, (char)((data[i][j].pitch)/12+4) + '0');
      }
      else mvaddstr(i+1, j*10+2, "---");
      // wave
      if(data[i][j].state & WAVE_B)
        mvaddch(i+1, j*10+7, data[i][j].wave+'0');
      else mvaddch(i+1, j*10+7, '-');
      // volume
      if(data[i][j].state & VOLUME_B)
        mvaddch(i+1, j*10+9, data[i][j].volume+'0');
      else mvaddch(i+1, j*10+9, '-');
    }
  }
  mvaddstr(33, 1, frame[0]);

  mvaddstr(0, 45, "Use wasd to move");
  mvaddstr(1, 45, "i to enter/exit insert mode");
  mvaddstr(2, 45, "zsxdcvgbhnjmk to enter notes in insert mode");
  mvaddstr(3, 45, "y to increase 1 octave, t to decrease 1 octave");
  mvaddstr(4, 45, "y and t outside insertmode to change octave globally");
  mvaddstr(5, 45, "columns: 1-pitch, 2-wave(press o), 3-volume");
  mvaddstr(6, 45, "1234 to change channels");
  mvaddstr(7, 45, "space to start/stop playing");
  mvaddstr(8, 45, "space in insert mode to set to blank");

  switch(mode){
  case normal:
    // input
    switch(input_key){
      case 'w': if(CY > 0) CY--; break;
      case 'a': if(CX > 0) CX--; break;
      case 's': if(CY < 31) CY++; break;
      case 'd': if(CX < 2) CX++; break;
      case 'i': mode = insert; break;
      case 'r': mode = replace; break;
      case 'y': if(global_octave < 4) global_octave++; break;
      case 't': if(global_octave > -4) global_octave--; break;
      case ' ': play_all(); break;
      default:
        if(input_key >= '1' && input_key <= '4') cursorchannel = input_key - '1';
        break;
    }
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
    if(input_key == 'r') mode = normal;
    goto normalreplace;
  case insert:
    if(input_key == 'i'){
      mode = normal;
      break;
    }
  normalreplace:
    switch(CX){
      case 0: // pitch
        mvaddch(CY+1, 5+cursorchannel*10, '<');
        mvaddch(CY+1, 6+cursorchannel*10, '=');
        if(input_key == ' '){
          data[CY][cursorchannel].state &= ~PITCH_B;
          if(CY < 31 && mode == insert) CY++;
          if(mode == replace) mode = normal;
        }
        else if(input_key == 't' && data[CY][cursorchannel].pitch > -48){
          data[CY][cursorchannel].pitch -= 12;
          data[CY][cursorchannel].state |= PITCH_B;
          if(mode == replace) mode = normal;
        }
        else if(input_key == 'y' && data[CY][cursorchannel].pitch < 48){
          data[CY][cursorchannel].pitch += 12;
          data[CY][cursorchannel].state |= PITCH_B;
          if(mode == replace) mode = normal;
        }
        else if(char_to_int(input_key) != -1){
          data[CY][cursorchannel].pitch = char_to_int(input_key) + global_octave*12;
          data[CY][cursorchannel].state |= PITCH_B;
          if(CY < 31 && mode == insert) CY++;
          if(mode == replace) mode = normal;
        }
        break;

      case 1: // wave
        mvaddch(CY+1, 5+cursorchannel*10, '=');
        mvaddch(CY+1, 6+cursorchannel*10, '>');
        if(input_key == ' '){
          data[CY][cursorchannel].state &= ~WAVE_B;
          mode = normal;
        }
      
        else if(input_key >= '1' && input_key <= '8'){
          data[CY][cursorchannel].wave = input_key - '0';
          data[CY][cursorchannel].state |= WAVE_B;
          mode = normal;
        }
        break;

      case 2: // volume
        if(input_key == ' '){
          data[CY][cursorchannel].state &= ~VOLUME_B;
          mode = normal;
        }
        else if(input_key >= '0' && input_key <= '8'){
          data[CY][cursorchannel].volume = input_key - '0';
          data[CY][cursorchannel].state |= VOLUME_B;
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
  for(int i=0; i<4; ++i) wave[i] = saved_waves[0];
  for(int i=0; i<32; ++i){
    for(int j=0; j<4; ++j){
      // apply changes only if they happen
      if(data[i][j].state & PITCH_B) pitch[j] = data[i][j].pitch;
      if(data[i][j].state & WAVE_B) wave[j] = saved_waves[data[i][j].wave - 1];
      else wave[j] = NULL;
      if(data[i][j].state & VOLUME_B) volume[j] = data[i][j].volume;
      play_change(wave[j], volume[j], pitch[j], j);
    }
    mvaddstr(i+1, 43, "<-");
    if(getch() == ' ') goto end;
    refresh();
    Pa_Sleep(100);
  }

end:
  // mute
  for(int j=0; j<4; ++j)
    play_change(NULL, 0, 0, j);
  return;
}
