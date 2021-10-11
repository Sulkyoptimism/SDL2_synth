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
    bool quit;
    bool debuglog = false;
    int alloc_count;

    // SDL
    const static int buffer_size = 4096; // must be a power of two, decrease to allow for a lower latency, increase to reduce risk of underrun.
    int block_size = 64;

    // voice
    int voiceNum;
    std::vector<voice> voices; //set in init

    const double pi = 3.14159265358979323846;
    const double chromatic_ratio = 1.059463094359295264562;
    int16_t* sine_wave_table;
    int octave;
    int max_note;
    int min_note;

    int* sample;

    //audio parameters
    int incoming_note;

  
    Synth(voice_params vp,double samp_rate, int t_length);
    void synth_init(int id);
    int assign_newnote(int new_note);
    void add_voice(int id, voice_params vp);
    void add_voice(voice voi);
    int* evaluate_samples();
    void key_press(int note, bool b);

    static void init_data(double samp_rate, int t_length);

    ~Synth();

private:

};
