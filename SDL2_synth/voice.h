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
    double lfo_rate;
    double lfo_phase_pos;
    int lfo_phase_int;
    float mod_factor;
    double amplitude_factor;
    double envelope_cursor = 0;
    double current_amp;
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
    double lfo_rate;
    double lfo_phase_pos;
    int lfo_phase_int;
    float mod_factor;
    double amplitude_factor;
    double envelope_cursor;
    double current_amp;

    bool key_pressed;

    int16_t sample[64];

    static voice_params default_params;
    // amplitude smoothing
    double envelope_data[4] = { 1.0f, 0.5f, 0.5f, 0.0f }; // ADSR amp range 0.0-1.0
    double envelope_speed_scale = 1; // set envelope speed 1-8
    double envelope_increment_base = 0; // this will be set in init_data based on current samplingrate.
    double target_amp = 0;
    double smoothing_amp_speed = 0.01;
    double smoothing_enabled = true;

    voice(double sample_rate, voice_params vp, int table_length);
    void init_voice(voice_params vp);
    int update_LFO_pos(double freq);
    void write_samples(long length);
    int16_t get_sample_from_table(int phase_int, int synthwave_mode, float pulse_width);
    double update_envelope();
    double get_envelope_amp_by_node(int base_node, double cursor);
    void key_press(int note, bool b);

    ~voice();

private:
};

