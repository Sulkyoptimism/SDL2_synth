#include "helper.h"


using namespace nlohmann;

voice_params helper::voiced_params = {
    false,//active
    false,
    -1,   //id //-1 non assign
    0,    //note
    0,    //detune
    0,    //mode
    0.5,  //pulsewidth
    0,    //phasepos
    0,    //phaseint
    0,   //lforate
    0,  //lfomode
    0,    //lfopos
    0,    //lfoint
    0.5,  //modfactor
    1    //ammpfac

};
synth_params helper::synthd_params = {
    -1,
    true,
    1,
    {
    voiced_params,
    voiced_params,
    voiced_params,
    voiced_params,
    voiced_params,
    voiced_params,
    voiced_params,
    voiced_params}

};
app_params helper::appd_params = {
    44100,
    4,
    {synthd_params,
    synthd_params,
    synthd_params,
    synthd_params,
    synthd_params,
    synthd_params,
    synthd_params,
    synthd_params}
};

app_params helper::saved_params = helper::appd_params;

app_params helper::load_dparams(std::string path)
{
    std::ifstream jsonf(path);
    json json_data;
    jsonf >> json_data;
    app_params ap = params_from_json(json_data);
    saved_params = ap;
    return ap;
}
app_params helper::params_from_json(json data) {
    app_params ap;

    ap.sample_rate = data.at("sample_rate");
    ap.number_synths = data.at("synth_number");
    std::vector<json> synth_list = data.at("sps");

    for (int i = 0; i < ap.number_synths; i++)
    {
        ap.sps[i].id = synth_list[i].at("synth_id");
        ap.sps[i].poly_mode = synth_list[i].at("poly_mode");
        ap.sps[i].polymax = synth_list[i].at("poly_max");
        std::vector<json> voice_list = synth_list[i].at("vps");


        for (int j = 0; j < 8; j++)
        {
            json j_voi;
            ap.sps[i].vps[j].active = voice_list[j].at("active"); //active
            ap.sps[i].vps[j].flagged = voice_list[j].at("flagged");//flagged
            ap.sps[i].vps[j].id = voice_list[j].at("id");   //id //-1 non assign
            ap.sps[i].vps[j].note = voice_list[j].at("note") ;    //note
            ap.sps[i].vps[j].detune = voice_list[j].at("detune") ;    //detune
            ap.sps[i].vps[j].mode = voice_list[j].at("mode");    //mode
            ap.sps[i].vps[j].pulse_width = voice_list[j].at("pulsewidth");  //pulsewidth
            ap.sps[i].vps[j].phase_position = voice_list[j].at("phasepos");    //phasepos
            ap.sps[i].vps[j].phase_int = voice_list[j].at("phaseint") ;    //phaseint
            ap.sps[i].vps[j].lfo_rate = voice_list[j].at("lforate") ;   //lforate
            ap.sps[i].vps[j].lfo_mode = voice_list[j].at("lfomode");
            ap.sps[i].vps[j].lfo_phase_pos = voice_list[j].at("lfopos");    //lfopos
            ap.sps[i].vps[j].lfo_phase_int = voice_list[j].at("lfoint");    //lfoint
            ap.sps[i].vps[j].mod_factor = voice_list[j].at("modfactor");  //modfactor
            ap.sps[i].vps[j].amplitude_factor = voice_list[j].at("ammpfac");    //ammpfac

        }
    }
    return ap;
}
void helper::export_dparams()
{
    json j_voi;
    j_voi["active"] = voiced_params.active; //active
    j_voi["flagged"] = voiced_params.flagged;//flagged
    j_voi["id"] = voiced_params.id;   //id //-1 non assign
    j_voi["note"] = voiced_params.note;    //note
    j_voi["detune"] = voiced_params.detune;    //detune
    j_voi["mode"] = voiced_params.mode;    //mode
    j_voi["pulsewidth"] = voiced_params.pulse_width;  //pulsewidth
    j_voi["phasepos"] = voiced_params.phase_position;    //phasepos
    j_voi["phaseint"] = voiced_params.phase_int;    //phaseint
    j_voi["lforate"] = voiced_params.lfo_rate;   //lforate
    j_voi["lfomode"] = voiced_params.lfo_mode;
    j_voi["lfopos"] = voiced_params.lfo_phase_pos;    //lfopos
    j_voi["lfoint"] = voiced_params.lfo_phase_int;    //lfoint
    j_voi["modfactor"] = voiced_params.mod_factor;  //modfactor
    j_voi["ammpfac"] = voiced_params.amplitude_factor;    //ammpfac

    json j_synth;
    j_synth["synth_id"] = synthd_params.id;
    j_synth["poly_mode"] = synthd_params.poly_mode;
    j_synth["poly_max"] = synthd_params.polymax;
    std::vector<json> json_list;
    for (int i = 0; i < 8; i++)
    {
        json_list.push_back(j_voi);
    }
    j_synth["vps"] = json_list;

    json j_out = {
        {"sample_rate", appd_params.sample_rate},
        {"synth_number", appd_params.number_synths},
    };

    json_list.clear();

    for (int i = 0; i < appd_params.number_synths; i++)
    {
        json_list.push_back(j_synth);
    }
    j_out["sps"] = json_list;

    std::ofstream file("default_params.json");
    file << j_out;   
}
void helper::export_params(app_params ap)
{


    json j_out = {
        {"sample_rate", ap.sample_rate},
        {"synth_number", ap.number_synths},
    };

    std::vector<json> jsynth_list;

    for (int i = 0; i < ap.number_synths; i++)
    {
        json j_synth;
        std::vector<json> jvoi_list;
        j_synth["synth_id"] = ap.sps[i].id;
        j_synth["poly_mode"] = ap.sps[i].poly_mode;
        j_synth["poly_max"] = ap.sps[i].polymax;

        for (int j = 0; j < 8; j++)
        {
            json j_voi;
            j_voi["active"] = voiced_params.active; //active
            j_voi["flagged"] = voiced_params.flagged;//flagged
            j_voi["id"] = voiced_params.id;   //id //-1 non assign
            j_voi["note"] = voiced_params.note;    //note
            j_voi["detune"] = voiced_params.detune;    //detune
            j_voi["mode"] = voiced_params.mode;    //mode
            j_voi["pulsewidth"] = voiced_params.pulse_width;  //pulsewidth
            j_voi["phasepos"] = voiced_params.phase_position;    //phasepos
            j_voi["phaseint"] = voiced_params.phase_int;    //phaseint
            j_voi["lforate"] = voiced_params.lfo_rate;   //lforate
            j_voi["lfomode"] = voiced_params.lfo_mode;
            j_voi["lfopos"] = voiced_params.lfo_phase_pos;    //lfopos
            j_voi["lfoint"] = voiced_params.lfo_phase_int;    //lfoint
            j_voi["modfactor"] = voiced_params.mod_factor;  //modfactor
            j_voi["ammpfac"] = voiced_params.amplitude_factor;    //ammpfac




            jvoi_list.push_back(j_voi);
        }
        j_synth["vps"] = jvoi_list;
        jsynth_list.push_back(j_synth);
    }
    j_out["sps"] = jsynth_list;
    std::ofstream file("saved_params.json");
    file << j_out;
}
helper::helper()
{

}

helper::~helper()
{
}




