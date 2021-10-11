#pragma once
#include <stdint.h>
#include <math.h>

enum synth_mode { SINE = 0, SQUARE = 1, TRI = 2, SAW = 3 };
namespace DSP {
    const  double pi = 3.14159265358979323846;
    const  double chromatic_ratio = 1.059463094359295264562;
}


class wavetable
{

public:
    static int16_t* sine_wave_table;
    static int16_t* saw_wave_table;

    static double sample_rate;
    static int table_length;

    static void init_data(double samp_rate, int t_length);
    static void delete_data();
    static void build_sine_table(int16_t* data, int wave_length);
    static void build_saw_table(int16_t* data, int wave_length);
    static int16_t square_from_sine(int index, float pulse_width);
    static int16_t triangle_from_sin(int index);
    static int16_t get_sample_from_table(int phase_int, int synthwave_mode, float pulse_width);

};

