#include "voice.h"
#include "manager.h"


voice_params voice::default_params = {
    false,//active
    -1,   //id //-1 non assign
    0,    //note
    0,    //mode
    0.5,  //pulsewidth
    0,    //phasepos
    0,    //phaseint
    10,   //lforate
    0,    //lfopos
    0,    //lfoint
    0.5,  //modfactor
    1,    //ammpfac
    0,    //envcursor
    0,    //curamp

};

voice::voice(double sample_rate, voice_params vp, int t_length)
{
    init_voice(vp);
    this->sample_rate = sample_rate;
    table_length = t_length;
    // set envelope increment size based on samplerate.
    envelope_increment_base = 1 / (double)(sample_rate / 2);

}

void voice::init_voice(voice_params vp)
{
    active = vp.active;
    id = vp.id;
    note = vp.note;
    mode = vp.mode;
    pulse_width = vp.pulse_width;
    phase_position = vp.phase_position;
    phase_int = vp.lfo_phase_int;
    lfo_rate = vp.lfo_rate;
    lfo_phase_pos = vp.lfo_phase_pos;
    lfo_phase_int = vp.lfo_phase_int;
    mod_factor = vp.mod_factor;    
    amplitude_factor = vp.amplitude_factor;
    envelope_cursor = vp.envelope_cursor;
    current_amp = vp.current_amp;
}


voice::~voice()
{
}

void voice::key_press(int note, bool b) {
    if (note == this->note) {
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
    switch (synthwave_mode)
    {
    case SINE:
        return manager::get_instance()->sine_wave_table[phase_int];
    case SQUARE:
        return manager::get_instance()->square_from_sine(phase_int, pulse_width);
    case TRI:
        return manager::get_instance()->triangle_from_sin(phase_int);
    case SAW:
        return manager::get_instance()->saw_wave_table[phase_int];
    default:
        break;
    }
}

void voice::write_samples(long length) {
    double pitch = manager::get_instance()->get_pitch(note);

    for (int i = 0; i < length; i ++ ) {
        if (active) {
            
            double lfo_out = manager::get_instance()->sine_wave_table[update_LFO_pos(lfo_rate)];
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
                    if (current_amp == 0) {
                        active = false;
                        this->init_voice(voice::default_params);
                    }
                }
                sample[i] = sample_back * current_amp;
            }   
            //printf("dad?");
        }
    }

}