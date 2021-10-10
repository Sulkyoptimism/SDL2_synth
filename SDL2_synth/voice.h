#pragma once
#include <stdint.h>
#include <vector>

struct voice_params {
    bool active;
    int id;
    int note;
    int mode;
    float pulse_width;
    double phase_position;
    int phase_int;
    double amplitude_factor;
    double lfo_rate;
    double lfo_phase_pos;
    int lfo_phase_int;
    float mod_factor;
    int16_t sample[64];
};



class voice
{
public:
    //general
    double sample_rate;
    int table_length;

    //voice
    bool active;
    int id;
    int note;
    int mode;
    float pulse_width;
    double phase_position;
    int phase_int;
    double amplitude_factor;
    double lfo_rate;
    double lfo_phase_pos;
    int lfo_phase_int;
    float mod_factor;
    int16_t sample[64];

    static voice_params default_params;

    voice(double sample_rate, voice_params vp, int table_length);
    void init_voice(voice_params vp);
    int update_LFO_pos(double freq);
    void write_samples(long length);

    ~voice();

private:
};

