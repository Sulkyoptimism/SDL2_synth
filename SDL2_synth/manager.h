#pragma once
#include "SDL2/SDL.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include "helper.h"
#include "Synth.h"
#include "voice.h"

//Global audio callback
namespace callback {
    void audio_callback(void* unused, Uint8* byte_stream, int byte_stream_length);
}

//namespace holder for const data
namespace DSP {
    const  double pi = 3.14159265358979323846;
    const  double chromatic_ratio = 1.059463094359295264562;
}

enum synthcontrol {SynthUp = -2, SynthDown = -3};

 class manager
{
public:
    manager();
    ~manager();

    //general
     bool quit;
     bool debuglog;

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
     int16_t* sine_wave_table;
     int* samples;

    // Synth synths[8];
     std::vector<Synth> synths;
     int synth_count;
     int octave;

    ////util func
    // 
    static manager* get_instance();
    //setup
    void set_up();
     void init_data(void);
    int setup_sdl_audio(void);
     void setup_sdl(void);

    //main loop
    void main_loop();

    void clean_up();
     void cleanup_data(void);
     void destroy_sdl(void);

    //audio func
     void write_samples_to_buffer(int16_t* s_byteStream, long begin, long end, long length);
     void build_sine_table(int16_t* data, int wave_length);
     double get_pitch(double note);

    //input func
     void check_sdl_events(SDL_Event event);
     void handle_key_down(SDL_Keysym* keysym);
     void handle_note_keys(SDL_Keysym* keysym);
     void print_note(int note);


private:
    static manager* singleton_instance;
};

