#include "global.h"
#include "play.h"
#include "render.h"
#include <ncurses.h>
#include <portaudio.h>
#define CX cursorx[cursorchannel]
#define CY cursory[cursorchannel]
#define PITCH_B 4
#define WAVE_B 2
#define VOLUME_B 1

static i8 cursorx[4] = {0};
static i8 cursory[4] = {0};
static i8 cursorchannel = 0;

static char board[2][42] = {
  "+=========+=========+=========+=========+",
  "|         |         |         |         |",
};

// y, channel, x[note, wave, volume, cmd]
static char playground[32][4][4];
static i8 insertmode = 0;

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

char *names[13] = {
  "A","A#","H","C","C#","D","D#","E","F","F#","G","G#"," "
};

static i8 octave = 0;

void render_default_init(void){
  // for(int i=0; i<32; ++i){
  //   for(int j=0; j<4; ++j) playground[i][j][0] = 0;
  // }
}

void render_default_refresh(void){
  mvaddstr(0, 1, board[0]);
  for(int i=0; i<32; ++i){
    mvaddstr(i+1, 1, board[1]);
    if(i%4 == 0) mvaddch(i+1, 0, '*');
    else if(i%2 == 0) mvaddch(i+1, 0, '-');

    for(int j=0; j<4; ++j){
      if(playground[i][j][3] & PITCH_B){
        mvaddstr(i+1, j*10+2, names[((int)playground[i][j][0]+48)%12]);
        mvaddch(i+1, j*10+4, (char)((playground[i][j][0])/12+4) + '0');
      }
      else mvaddstr(i+1, j*10+2, "---");
      if(playground[i][j][3] & WAVE_B)
        mvaddch(i+1, j*10+7, playground[i][j][1]+'0');
      else mvaddch(i+1, j*10+7, '-');
      if(playground[i][j][3] & VOLUME_B)
        mvaddch(i+1, j*10+9, playground[i][j][2]+'0');
      else mvaddch(i+1, j*10+9, '-');
    }
  }
  mvaddstr(33, 1, board[0]);

  mvaddstr(0, 45, "Use wasd to move");
  mvaddstr(1, 45, "i to enter insert mode/exit");
  mvaddstr(2, 45, "zsxdcvgbhnjmk to enter notes in insert mode");
  mvaddstr(3, 45, "y to increase 1 octave, t to decrease 1 octave");
  mvaddstr(4, 45, "y and t outside insertmode to change octave globally");
  mvaddstr(5, 45, "columns: 1-pitch, 2-wave(press o), 3-volume");
  mvaddstr(6, 45, "1234 to change channels");

  if(!insertmode){
    if(input_key >= '1' && input_key <= '4') cursorchannel = input_key - '1';
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

    switch(input_key){
      case 'w':
        if(CY > 0) CY--;
        break;
      case 'a':
        if(CX > 0) CX--;
        break;
      case 's':
        if(CY < 31) CY++;
        break;
      case 'd':
        if(CX < 2) CX++;
        break;
      case 'i':
        insertmode = 1;
        break;
      case 'y':
        if(octave < 4) octave++;
        break;
      case 't':
        if(octave > -4) octave--;
        break;
      case ' ':
        play_all();
        break;
      default:
        break;
    }
  }

  else{
    if(input_key == 'i') insertmode = 0;
    else if(CX == 0 && input_key == 't' && playground[CY][cursorchannel][0] > -48){
      playground[CY][cursorchannel][0] -= 12;
      playground[CY][cursorchannel][3] |= PITCH_B;
    }
    else if(CX == 0 && input_key == 'y' && playground[CY][cursorchannel][0] < 48){
      playground[CY][cursorchannel][0] += 12;
      playground[CY][cursorchannel][3] |= PITCH_B;
    }

    else if(CX == 0 && input_key >= 'a' && input_key <= 'z' && input_key != 'y' && input_key != 't'){
      playground[CY][cursorchannel][0] = char_to_int(input_key) + octave*12;
      playground[CY][cursorchannel][3] |= PITCH_B;
    }

    else if(CX == 1 && input_key >= '1' && input_key <= '8'){
      playground[CY][cursorchannel][1] = input_key - '0';
      playground[CY][cursorchannel][3] |= WAVE_B;
    }
    else if(CX == 2 && input_key >= '0' && input_key <= '8'){
      playground[CY][cursorchannel][2] = input_key - '0';
      playground[CY][cursorchannel][3] |= VOLUME_B;
    }
  }
}

void play_all(void){
  int pitch[4] = {0};
  int volume[4] = {0};
  i8 *wave[4];
  for(int i=0; i<4; ++i) wave[i] = saved_waves[0];
  for(int i=0; i<32; ++i){
    for(int j=0; j<4; ++j){
      if(playground[i][j][3] & PITCH_B) pitch[j] = playground[i][j][0];
      if(playground[i][j][3] & WAVE_B) wave[j] = saved_waves[playground[i][j][1] - 1];
      if(playground[i][j][3] & VOLUME_B) volume[j] = playground[i][j][2];
      play_change(wave[j], volume[j], pitch[j], j);
    }
    mvaddstr(i+1, 43, "<-");
    refresh();
    Pa_Sleep(100);
  }
}
