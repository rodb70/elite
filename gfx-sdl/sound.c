/*
 * sound.c
 *
 *  Created on: 18 Dec 2023
 *      Author: rod
 */
#include "sound.h"

#include <stdio.h>

#define NUM_SAMPLES 14

static int sound_on;

struct sound_sample
{
    void *sample;
    char filename[ 256 ];
    int runtime;
    int timeleft;
};

struct sound_sample sample_list[ NUM_SAMPLES ] =
{
    { NULL, "launch.wav",    32, 0 },
    { NULL, "crash.wav",      7, 0 },
    { NULL, "dock.wav",      36, 0 },
    { NULL, "gameover.wav",  24, 0 },
    { NULL, "pulse.wav",      4, 0 },
    { NULL, "hitem.wav",      4, 0 },
    { NULL, "explode.wav",   23, 0 },
    { NULL, "ecm.wav",       23, 0 },
    { NULL, "missile.wav",   25, 0 },
    { NULL, "hyper.wav",     37, 0 },
    { NULL, "incom1.wav",     4, 0 },
    { NULL, "incom2.wav",     5, 0 },
    { NULL, "beep.wav",       2, 0 },
    { NULL, "boop.wav",       7, 0 }
};

void snd_sound_startup (void)
{
    /* Install a sound driver.. */
    sound_on = 1;

    if (!sound_on)
    {
        return;
    }

}

void snd_sound_shutdown (void)
{
    if (!sound_on)
    {
        return;
    }

}

void snd_play_sample (int sample_no)
{
    (void)sample_no;
    if (!sound_on)
    {
        return;
    }

}

void snd_play_midi (int midi_no, int repeat)
{
    (void)repeat;
    (void)midi_no;

}

void snd_update_sound (void)
{
    if (!sound_on)
    {
        return;
    }

}

void snd_stop_midi (void)
{
    if (!sound_on)
    {
        return;
    }

}
