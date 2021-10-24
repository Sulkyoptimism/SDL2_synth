#include "voice.h"
//
//
//voice_params voice::default_params = {
//    false,//active
//    false,
//    -1,   //id //-1 non assign
//    0,    //note
//    0,    //detune
//    0,    //mode
//    0.5,  //pulsewidth
//    0,    //phasepos
//    0,    //phaseint
//    10,   //lforate
//    0,    //lfopos
//    0,    //lfoint
//    0.5  //modfactor
//
//
//};

double voice::sample_rate = 1;
int voice::table_length = 1024;
int16_t* voice::sine_wave_table = new int16_t[table_length];
int16_t* voice::saw_wave_table = new int16_t[table_length];
bool voice::is_init = false;

voice::voice(double sample_rate, int t_length)
{
    this->sample_rate = sample_rate;
    table_length = t_length;
    voice_params test_params = default_params;

    // set envelope increment size based on samplerate.
    envelope_increment_base = 1 / (double)(sample_rate / 2);
    //int samp = sine_wave_table[3];
}

void voice::init_voice( int parent_id, voice_params vp)
{
    default_params = vp;
    this->parent_id = parent_id;
    current_amp = 0;
    envelope_cursor = 0;
    active = vp.active;
    flagged = vp.flagged;
    id = vp.id;
    note = vp.note;
    detune = vp.detune;
    mode = vp.mode;
    pulse_width = vp.pulse_width;
    phase_position = vp.phase_position;
    phase_int = vp.lfo_phase_int;
    lfo_rate = vp.lfo_rate;
    lfo_mode = vp.lfo_mode;
    lfo_phase_pos = vp.lfo_phase_pos;
    lfo_phase_int = vp.lfo_phase_int;
    mod_factor = vp.mod_factor;    
    amplitude_factor = vp.amplitude_factor;

}


voice::~voice()
{

}

void voice::key_press(int note, bool b, bool poly_over) {
    if (!voice::is_init) {
        init_data();
    }
    if (note == this->note || poly_over) {
        key_pressed = b;
    }
}


int voice::update_LFO_pos(double freq) {
    double phase_increment = (freq / sample_rate) * table_length;
    lfo_phase_pos += phase_increment;
    lfo_phase_int = (int)lfo_phase_pos;
    if (lfo_phase_pos >= table_length) {
        double diff = lfo_phase_pos - table_length;
        lfo_phase_pos = diff;
        lfo_phase_int = (int)diff;
    }

    return lfo_phase_int;
}


double voice::get_envelope_amp_by_node(int base_node, double cursor) {

    // interpolate amp value for the current cursor position.

    double n1 = base_node;
    double n2 = base_node + 1;
    double relative_cursor_pos = (cursor - n1) / (n2 - n1);
    double amp_diff = (envelope_data[base_node + 1] - envelope_data[base_node]);
    double amp = envelope_data[base_node] + (relative_cursor_pos * amp_diff);
    return amp;
}

double voice::update_envelope() {

    // advance envelope cursor and return the target amplitude value.

    double amp = 0;
    if (key_pressed && envelope_cursor < 3 && envelope_cursor > 2) {
        // if a note key is longpressed and cursor is in range, stay for sustain.
        amp = get_envelope_amp_by_node(2, envelope_cursor);
    }
    else {
        double speed_multiplier = pow(2, envelope_speed_scale);
        double cursor_inc = envelope_increment_base * speed_multiplier;
        envelope_cursor += cursor_inc;
        if (envelope_cursor < 1) {
            amp = get_envelope_amp_by_node(0, envelope_cursor);
        }
        else if (envelope_cursor < 2) {
            amp = get_envelope_amp_by_node(1, envelope_cursor);
        }
        else if (envelope_cursor < 3) {
            amp = get_envelope_amp_by_node(2, envelope_cursor);
        }
        else {
            amp = envelope_data[3];
        }
    }
    return amp;
}


int16_t voice::get_sample_from_table(int phase_int, int synthwave_mode, float pulse_width) {
    int16_t samp = 0;
    switch (synthwave_mode)
    {
    case SINE:
        samp = sine_wave_table[phase_int];
        return samp;
    case SQUARE:
        return square_from_sine(phase_int, pulse_width);
    case TRI:
        return triangle_from_sin(phase_int);
    case SAW:
        return saw_wave_table[phase_int];
    default:
        break;
    }
}
void voice::init_data() {
    if (!is_init) {
        sine_wave_table = new int16_t[table_length];
        saw_wave_table = new int16_t[table_length];
        sine_wave_table = build_sine_table(table_length);
        build_saw_table(saw_wave_table, table_length);
        is_init = true;
        //int samp = sine_wave_table[3]; //sinewave mem test
    }

}
void voice::delete_data() {
    delete[] sine_wave_table;
    delete[] saw_wave_table;
}
int16_t* voice::build_sine_table( int wave_length) {

    /*
        Build sine table to use as oscillator:
        Generate a 16bit signed integer sinewave table with 1024 samples.
        This table will be used to produce the notes.
        Different notes will be created by stepping through
        the table at different intervals (phase).
    */
    //manager::get_instance()->myfile << "Logging the sinewave table:\n\n\n";
    int16_t* buf = new int16_t[wave_length];
    double phase_increment = (2.0f * DSP::pi) / (double)wave_length;
    double current_phase = 0;
    for (int i = 0; i < wave_length; i++) {
        int sample = (int)(sin(current_phase) * INT16_MAX);
        buf[i] = (int16_t)sample;
        //manager::get_instance()->myfile << data[i] << "\n";
        current_phase += phase_increment;
    }
    //manager::get_instance()->myfile << "\nLogged the sinewave table:\n";
    return buf;
}
void voice::build_saw_table(int16_t* data, int wave_length) {
    double factor = ((INT16_MAX * 2) - 1) / wave_length;
    for (int i = 0; i < wave_length; i++) {
        int sample = (i * factor) - (INT16_MAX - 1);
        data[i] = (int16_t)sample;
    }
}
int16_t voice::square_from_sine(int index, float pulse_width) {
    int temp_int = sine_wave_table[index];
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
int16_t voice::triangle_from_sin(int index) {
    double temp = ((double)sine_wave_table[index]) / (double)INT16_MAX;
    return (int16_t)(asin(temp) * INT16_MAX);
}



void voice::write_samples(long length) {

    double pitch = get_pitch(note);

    for (int i = 0; i < length; i ++ ) {
        if (active) {
            
            double lfo_out = get_sample_from_table(update_LFO_pos(lfo_rate), lfo_mode, 0.5);
            double lfo_norm = lfo_out / (float)(INT16_MAX+1); //32768.0f;
            //double lfo_norm = 0;
            double pitch_mod = (pitch * mod_factor) * lfo_norm;

            double phase_increment = ((pitch + pitch_mod) / sample_rate) * table_length;

            phase_position += phase_increment;
            phase_int = (int)phase_position;
            if (phase_position >= table_length) {
                double diff = phase_position - table_length;
                phase_position = diff;
                phase_int = (int)diff;
            }

            if (phase_int < table_length && phase_int > -1) {
                int16_t sample_back = get_sample_from_table(phase_int, mode, pulse_width) * amplitude_factor;
                if (smoothing_enabled) {
                    target_amp = update_envelope();
                    // move current amp towards target amp for a smoother transition.
                    if (current_amp < target_amp) {
                        current_amp += smoothing_amp_speed;
                        if (current_amp > target_amp) {
                            current_amp = target_amp;
                        }
                    }
                    else if (current_amp > target_amp) {
                        current_amp -= smoothing_amp_speed;
                        if (current_amp < target_amp) {
                            current_amp = target_amp;
                        }
                    }
                }
                else {
                    current_amp = target_amp;

                }
                if (current_amp == 0) {
                    active = false;
                    this->init_voice(parent_id, default_params);
                }
                sample[i] = sample_back * current_amp;
            }   
            //printf("dad?");
        }
    }

}

double voice::get_pitch(double note) {

    /*
        Calculate pitch from note value.
        offset note by 57 halfnotes to get correct pitch from the range we have chosen for the notes.
    */
    double p = pow(DSP::chromatic_ratio, note - 57);
    p *= 440;
    return p;
}
