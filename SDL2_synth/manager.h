#pragma once
#include "SDL2/SDL.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include "Synth.h"
#include <vector>
#include "json.hpp"

//Global audio callback
namespace callback {
    void audio_callback(void* unused, Uint8* byte_stream, int byte_stream_length);
}

//namespace holder for const data


enum synth_control {SynthUp = -2, SynthDown = -3};

//struct app_params {
//    double sample_rate;
//    int number_synths;
//    synth_params sps[8];
//};

 class manager
{
public:
    ////util func
    static app_params default_params;
    static manager* get_instance();
    manager();
    ~manager();

    //general
    bool quit;
    bool debuglog;
    int last_note = -1;

    std::ofstream myfile;

    //SDL
    SDL_Window* window;
    SDL_Renderer* renderer;
    SDL_GLContext context;
    SDL_Event event;
    SDL_AudioDeviceID audio_device;
    SDL_AudioSpec audio_spec;

    //Audio
    int buffer_size;
    double sample_rate;
    int table_length;

    int* samples;
    std::vector<int> held_notes;

    // Synth synths[8];
    int max_num_synths =  8;
    std::vector<Synth> synths;
    int synth_count;
    int octave;


    //setup
    void set_up(app_params ap);
    int setup_sdl_audio(void);
    void setup_sdl(void);

    //main loop
    void main_loop();

    void clean_up();
    void cleanup_data(void);
    void destroy_sdl(void);

    //audio func
    void write_samples_to_buffer(int16_t* s_byteStream, long begin, long end, long length);
    double get_pitch(double note);

    //input func
    void check_sdl_events(SDL_Event event);
    void handle_key_down(SDL_Keysym* keysym);
    void handle_key_up(SDL_Keysym* keysym);
    void handle_note_keys(SDL_Keysym* keysym);
    void print_note(int note);
    void key_press(int note, bool b);


private:
    static manager* singleton_instance;
};

