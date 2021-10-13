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

//struct synth_params {
//    int id;
//    bool poly_mode;
//    voice_params vps[8];
//
//};

class Synth
{
public:

    // general
    bool active;
    int id;
    bool poly_mode = true;

    double sample_rate;
    int table_length;

    // voice
    std::vector<voice> voices; //set in init
    synth_params default_params;

    int octave = 2;
    int max_note = 131;
    int min_note = 12;
    int* sample;

  
    Synth(double sample_rate, int table_length);
    static synth_params get_defaults();
    void init_synth(synth_params sp);
    void synth_activate(int id);
    int assign_newnote(int new_note);
    int* evaluate_samples(int block_size);
    void key_press(int note, bool b);
    void clean_up();
    ~Synth();

private:

};
