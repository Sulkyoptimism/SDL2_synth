#include <stdio.h>
#include <stdlib.h>
#include "SDL2/SDL.h"
#include <fstream>
#include "json.hpp"
#include <string>



struct voice_params {
    bool active;
    bool flagged;
    int id;
    int note;
    float detune;
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
};

struct synth_params {
    int id;
    bool poly_mode;
    voice_params vps[8];

};

struct app_params {
    double sample_rate;
    int number_synths;
    synth_params sps[8];
};

using namespace nlohmann;

class helper
{
public:
	json json_file;

    static voice_params voiced_params;
    static synth_params synthd_params;
    static app_params appd_params;

    static app_params saved_params;

    static app_params load_dparams(std::string path);


    static void export_dparams();
    static void export_params(app_params);
    helper();
	~helper();

private:
    static app_params params_from_json(json data);

};


