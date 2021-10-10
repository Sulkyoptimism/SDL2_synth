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
    1,    //ammpfac
    10,   //lforate
    0,    //lfopos
    0,    //lfoint
    0.5   //modfactor

};

voice::voice(double sample_rate, voice_params vp, int t_length)
{
    init_voice(vp);
    this->sample_rate = sample_rate;
    table_length = t_length;

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
    amplitude_factor = vp.amplitude_factor;
    lfo_rate = vp.lfo_rate;
    lfo_phase_pos = vp.lfo_phase_pos;
    lfo_phase_int = vp.lfo_phase_int;
    mod_factor = vp.mod_factor;    
}


voice::~voice()
{
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

int16_t get_sample_from_table(int phase_int, int synthwave_mode, float pulse_width) {
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
            double lfo_norm = lfo_out / (float)(SDL_MAX_SINT16+1); //32768.0f;
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
                sample[i] = sample_back;
            }   
            //printf("dad?");
        }
    }

}