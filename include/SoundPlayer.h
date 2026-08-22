#ifndef SOUNDPLAYER_H
#define SOUNDPLAYER_H

#include "Toolbox.h"

//Creates a inturpt procedure for timer 1
void timerInturupt() __attribute__ ((section(".iwram")));

//Setup the inturupt to go to the given function
void setUpInturupt(fnptr function, u32 inturupts);

//Turns on all the sound related functions
void enableSound();

//Sets the sample rate for timer 0#endif
void setSampleRate(int rate);

//Plays a sound on the given channel for the given time
void playSound(const signed char* sound, int length, int channel);
int isPlaying(int channel);

#endif