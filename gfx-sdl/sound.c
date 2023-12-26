/*
 * sound.c
 *
 *  Created on: 18 Dec 2023
 *      Author: rod
 */
#include "sound.h"

#define NUM_SAMPLES 14

static int sound_on;

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
