#pragma once
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <vector>
#include "voice.h"


class Synth
{
public:

    // general
    bool active;
    int id;
    bool poly_mode = true;

    // voice
    std::vector<voice> voices; //set in init

    int octave = 2;
    int max_note = 131;
    int min_note = 12;
    int* sample;

  
    Synth(voice_params vp, double sample_rate, int table_length);
    void synth_init(int id);
    int assign_newnote(int new_note);
    void add_voice(int id, voice_params vp);
    void add_voice(voice voi);
    int* evaluate_samples(int block_size);
    void key_press(int note, bool b);
    void clean_up();
    ~Synth();

private:

};
