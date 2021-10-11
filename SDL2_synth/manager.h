#pragma once
#include "SDL2/SDL.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include "helper.h"
#include "Synth.h"
#include <vector>

//Global audio callback
namespace callback {
    void audio_callback(void* unused, Uint8* byte_stream, int byte_stream_length);
}


enum synth_control {SynthUp = -2, SynthDown = -3};

 class manager
{
public:
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
     int16_t* sine_wave_table;
     int16_t* saw_wave_table;
     int* samples;
     std::vector<int> held_notes;

    // Synth synths[8];
    const int max_num_synths =  4;
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
     void build_saw_table(int16_t* data, int wave_length);
     int16_t square_from_sine(int index, float pulse_width);
     int16_t triangle_from_sin(int index);
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

