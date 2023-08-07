#include "play.h"
#include <curses.h>
#include <ncurses.h>
#include <string.h>
#define MAGIC_NUMBER 1.05946309435929526456

static i8 waves[4][32];
PaStream *streams[4];
PaError err;
static double increment[4];
static int amp[4];
static double test[4];
static char thing[4];

static int play_callback( const void *inputBuffer,
                       void *outputBuffer,
                       unsigned long framesPerBuffer,
                       const PaStreamCallbackTimeInfo* timeInfo,
                       PaStreamCallbackFlags statusFlags,
                       void *userData ){
    (void) inputBuffer; /* Prevent unused variable warning. */
    (void) timeInfo;
    (void) statusFlags;
    (void) framesPerBuffer;
    i8 *out = outputBuffer;
    i8 channel = *(i8*)userData;

    for(int i=0; i<256; ++i){
        test[channel] += increment[channel];
        out[i] = waves[channel][((int)test[channel])%32] * amp[channel];
        if(test[channel] > 32) test[channel] -= 32;
    }

    return 0;
}

void play_init(void){
    for(int i=0; i<4; ++i) thing[i] = i;
    /* Initialize library before making any other calls. */
    err = Pa_Initialize();
    if( err != paNoError ) goto error;
    
    /* Open an audio I/O stream. */
    for(int i=0; i<4; ++i){
        err = Pa_OpenDefaultStream( &streams[i],
                                    0,          /* no input channels */
                                    1,
                                    paInt8,  // 8 bit output
                                    44100,
                                    256,        /* frames per buffer */
                                    play_callback,
                                    &thing[i]);
        if( err != paNoError ) goto error;
    
        err = Pa_StartStream( streams[i] );
        if( err != paNoError ) goto error;
    }

    /* Sleep for several seconds. */
    // Pa_Sleep(NUM_SECONDS*1000);

    printf("Test finished.\n");
    return;
error:
    Pa_Terminate();
    fprintf( stderr, "An error occured while using the portaudio stream\n" );
    fprintf( stderr, "Error number: %d\n", err );
    fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
}

void play_stop(void){
    for(int i=0; i<4; ++i){
        err = Pa_StopStream( streams[i] );
        if( err != paNoError ) goto error;
        err = Pa_CloseStream( streams[i] );
        if( err != paNoError ) goto error;
    }
    Pa_Terminate();
    return;
error:
    Pa_Terminate();
    fprintf( stderr, "An error occured while using the portaudio stream\n" );
    fprintf( stderr, "Error number: %d\n", err );
    fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
}

void play_change(i8 wave_in[32], int amplitude_in, int pitch_in, i8 channel){ //pitch in half steps from A4
    double freq = 440.0 * pow(MAGIC_NUMBER, pitch_in);
    double samples_per_wave = 44100.0/freq;

    increment[channel] = 32.0/samples_per_wave;
    amp[channel] = amplitude_in;
    test[channel] = 0;
    memcpy(waves[channel], wave_in, 32);
}
