#include "play.h"
#define MAGIC_NUMBER 1.05946309435929526456

PaStream *streams[4];
PaError err;
struct channel{
    i8 wave[32];
    double increment;
    double test;
    int amp;
    PaStream *stream;
};
static struct channel channels[4];

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
    struct channel *c = (struct channel*)userData;

    for(int i=0; i<256; ++i){
        c->test += c->increment;
        out[i] = c->wave[((int)c->test)%32] * c->amp;
        if(c->test > 32) c->test -= 32;
    }

    return 0;
}

void play_init(void){
    /* Initialize library before making any other calls. */
    for(int i=0; i<4; ++i){
        channels[i].increment = 0;
        channels[i].test = 0;
        channels[i].amp = 0;
    }
    err = Pa_Initialize();
    if( err != paNoError ) goto error;
    
    /* Open an audio I/O stream. */
    for(int i=0; i<4; ++i){
        err = Pa_OpenDefaultStream( &channels[i].stream,
                                    0,          /* no input channels */
                                    1,
                                    paInt8,  // 8 bit output
                                    44100,
                                    256,        /* frames per buffer */
                                    play_callback,
                                    &channels[i]);
        if( err != paNoError ) goto error;
    
        err = Pa_StartStream( channels[i].stream );
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
        err = Pa_StopStream( channels[i].stream );
        if( err != paNoError ) goto error;
        err = Pa_CloseStream( channels[i].stream );
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

    channels[channel].increment = 32.0/samples_per_wave;
    channels[channel].amp = amplitude_in;
    if(wave_in != NULL) memcpy(channels[channel].wave, wave_in, 32);
}
