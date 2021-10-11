#include "wavetables.h"

wavetables* wavetables::singleton_instance = new wavetables();

wavetables::wavetables() {}

wavetables* wavetables::get_instance() {
    if (singleton_instance == nullptr) {
        singleton_instance = new wavetables();
    }
    return singleton_instance;
}

void wavetables::init(double sample_rate, int table_length) {
    if (!init_called) {
        sine_wave_table = new int16_t[table_length];
        saw_wave_table = new int16_t[table_length];
        this->sample_rate = sample_rate;
        this->table_length = table_length;
        build_sine_table(sine_wave_table, table_length);
        build_saw_table(saw_wave_table, table_length);
        init_called = true;
    }
}
void wavetables::build_sine_table(int16_t* data, int wave_length) {

    /*
        Build sine table to use as oscillator:
        Generate a 16bit signed integer sinewave table with 1024 samples.
        This table will be used to produce the notes.
        Different notes will be created by stepping through
        the table at different intervals (phase).
    */
    //myfile << "Logging the sinewave table:\n\n\n";

    double phase_increment = (2.0f * DSP::pi) / (double)wave_length;
    double current_phase = 0;
    for (int i = 0; i < wave_length; i++) {
        int sample = (int)(sin(current_phase) * INT16_MAX);
        data[i] = (int16_t)sample;
        //myfile << data[i] << "\n";
        current_phase += phase_increment;
    }
    //myfile << "\nLogged the sinewave table:\n";

}
void wavetables::build_saw_table(int16_t* data, int wave_length) {
    double factor = ((INT16_MAX * 2) - 1) / wave_length;
    for (int i = 0; i < wave_length; i++) {
        int sample = (i * factor) - (INT16_MAX - 1);
        data[i] = (int16_t)sample;
    }
}
int16_t wavetables::square_from_sine(int index, float pulse_width) {
    int temp_int = wavetables::get_instance()->sine_wave_table[index];
    float factor = (1 - pulse_width) * INT16_MAX;
    if (temp_int > factor) {
        temp_int = INT16_MAX;
    }
    else if (temp_int < -factor) {
        temp_int = (-INT16_MAX) + 1;
    }
    else {
        temp_int = 0;
    }
    return (int16_t)temp_int;

}
int16_t wavetables::triangle_from_sin(int index) {
    double temp = ((double)wavetables::get_instance()->sine_wave_table[index]) / (double)INT16_MAX;
    return (int16_t)(asin(temp) * INT16_MAX);
}


int16_t wavetables::get_sample_from_table(int phase_int, int synthwave_mode, float pulse_width) {
    switch (synthwave_mode)
    {
    case SINE:
        return wavetables::get_instance()->sine_wave_table[phase_int];
    case SQUARE:
        return wavetables::get_instance()->square_from_sine(phase_int, pulse_width);
    case TRI:
        return wavetables::get_instance()->triangle_from_sin(phase_int);
    case SAW:
        return wavetables::get_instance()->saw_wave_table[phase_int];
    default:
        break;
    }
}