#include "include/play.h"

int main(void){
    play_init();
    i8 wave[16] = {-8,-8,-8,-8,-8,-8,-8,-8, 8, 8, 8, 8, 8, 8, 8, 8};
    play_change(wave, 8, 0);
    Pa_Sleep(5000);
    play_stop();
    return 0;
}
