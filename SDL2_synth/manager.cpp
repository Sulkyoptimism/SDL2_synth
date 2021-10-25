#include "manager.h"

manager* manager::singleton_instance = new manager();

//app_params manager::default_params = {
//    44100,
//    4,
//    {Synth::default_params,
//    Synth::default_params,
//    Synth::default_params,
//    Synth::default_params,
//    Synth::default_params,
//    Synth::default_params,
//    Synth::default_params,
//    Synth::default_params}
//};


manager::manager()
{
    myfile.open("outlog.txt");
    quit = false;
    debuglog = false;
    window = NULL;
    renderer = NULL;
    octave = 2;
    synth_count = 0;
    table_length = 1024;
    samples = nullptr;



}

manager::~manager()
{
    myfile.close();
}

manager* manager::get_instance() {
    if (singleton_instance ==nullptr) {
        singleton_instance = new manager();
    }
    return singleton_instance;
}

void manager::main_loop() {
    // check for keyboard events etc.
    check_sdl_events(event);


    // update screen.
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    SDL_Delay(16);
}

void manager::check_sdl_events(SDL_Event event) {

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            quit = true;
            break;
        case SDL_KEYDOWN:
            handle_key_down(&event.key.keysym);
            break;
        case SDL_KEYUP:
            handle_key_up(&event.key.keysym);
            break;
        }
    }
}


static int get_key(SDL_Keysym* keysym)
{
    int new_note = -1; //note
    switch (keysym->sym) {
    case SDLK_z:
        new_note = 12;
        break;
    case SDLK_s:
        new_note = 13;
        break;
    case SDLK_x:
        new_note = 14;
        break;
    case SDLK_d:
        new_note = 15;
        break;
    case SDLK_c:
        new_note = 16;
        break;
    case SDLK_v:
        new_note = 17;
        break;
    case SDLK_g:
        new_note = 18;
        break;
    case SDLK_b:
        new_note = 19;
        break;
    case SDLK_h:
        new_note = 20;
        break;
    case SDLK_n:
        new_note = 21;
        break;
    case SDLK_j:
        new_note = 22;
        break;
    case SDLK_m:
        new_note = 23;
        break;
    case SDLK_COMMA:
        new_note = 24;
        break;
    case SDLK_l:
        new_note = 25;
        break;
    case SDLK_PERIOD:
        new_note = 26;
        break;

        //upper keyboard
    case SDLK_q:
        new_note = 24;
        break;
    case SDLK_2:
        new_note = 25;
        break;
    case SDLK_w:
        new_note = 26;
        break;
    case SDLK_3:
        new_note = 27;
        break;
    case SDLK_e:
        new_note = 28;
        break;
    case SDLK_r:
        new_note = 29;
        break;
    case SDLK_5:
        new_note = 30;
        break;
    case SDLK_t:
        new_note = 31;
        break;
    case SDLK_6:
        new_note = 32;
        break;
    case SDLK_y:
        new_note = 33;
        break;
    case SDLK_7:
        new_note = 34;
        break;
    case SDLK_u:
        new_note = 35;
        break;
    case SDLK_i:
        new_note = 36;
        break;
    case SDLK_9:
        new_note = 37;
        break;
    case SDLK_o:
        new_note = 38;
        break;
    case SDLK_0:
        new_note = 39;
        break;
    case SDLK_p:
        new_note = 40;
        break;
    case SDLK_RIGHT:
        new_note = SynthUp;
        break;
    case SDLK_LEFT:
        new_note = SynthDown;
        break;
    default:
        break;
    }
    return new_note;
}

void manager::handle_key_down(SDL_Keysym* keysym) {

    handle_note_keys(keysym);
}

void manager::key_press(int note, int synthid, bool b) {
     synths[synthid].key_press(note, b);
}

void manager::handle_key_up(SDL_Keysym* keysym) {

    switch (keysym->sym) {
    case SDLK_PLUS:
        break;
    case SDLK_MINUS:
        break;
    default:
        int note = get_key(keysym);
        note += (octave * 12);

        held_notes.erase(std::remove(held_notes.begin(), held_notes.end(), note), held_notes.end());
        //keypress false
        key_press(note, synth_count, false);
        last_note = -1;
        
        break;
    }
}

void manager::handle_note_keys(SDL_Keysym* keysym) {
    // change note or octave depending on which key is pressed.
    int temp_note = get_key(keysym);
    int new_note = temp_note;
    temp_note += (octave * 12);
    bool is_new = false;
    std::vector<int>::iterator it;
    it = std::find(held_notes.begin(), held_notes.end(), temp_note);
    if (it != held_notes.end()) {}
    else {
        is_new = true;
    }

    //if new note is valid and is new
    if (is_new && new_note > -1) {
        if (synths[synth_count].active && synths[synth_count].poly_mode) {
            new_note += (octave * 12);
            int err = synths[synth_count].assign_newnote(new_note);
            printf("note base: %i, pitch: %f, on synth: %i\n", new_note, get_pitch(new_note), synth_count);
            held_notes.push_back(new_note);
        }
        else if(held_notes.size() == 0){
            new_note += (octave * 12);
            //start new voice if 
            synths[synth_count].synth_activate(synth_count);
            int err = synths[synth_count].assign_newnote(new_note);
            printf("note base: %i, pitch: %f, on synth: %i\n", new_note, get_pitch(new_note), synth_count);
            held_notes.push_back(new_note);
        }

        //keypress true
        key_press(new_note, synth_count, true);

    }
    else if (new_note == SynthUp)
    {
        if (synth_count > 3) {
            synth_count = 3;
        }
        else {
            synth_count++;
        }
   
        printf("Synth changer, Synth changed to %i, code: %i\n", synth_count, SynthUp);

    }
    else if (new_note == SynthDown)
    {
        if (synth_count < 0){
            synth_count = 0;
        }
        else {
            synth_count--;
        }

        printf("Synth changer, Synth changed to %i, code: %i\n", synth_count, SynthDown);

    }
}

void manager::print_note(int note) {

    int note_without_octave = note % 12;
    int note_octave = (note / 12) - 1;
    char note_chars = NULL;
    switch (note_without_octave) {
    case 0:
        note_chars = 'C-';
        break;
    case 1:
        note_chars = 'C#';
        break;
    case 2:
        note_chars = 'D-';
        break;
    case 3:
        note_chars = 'D#';
        break;
    case 4:
        note_chars = 'E-';
        break;
    case 5:
        note_chars = 'F-';
        break;
    case 6:
        note_chars = 'F#';
        break;
    case 7:
        note_chars = 'G-';
        break;
    case 8:
        note_chars = 'G#';
        break;
    case 9:
        note_chars = 'A-';
        break;
    case 10:
        note_chars = 'A#';
        break;
    case 11:
        note_chars = 'B-';
        break;
    }
    printf("note: %s%d pitch: %fHz\n", note_chars, note_octave, get_pitch(note));
}
double manager::get_pitch(double note) {

    /*
        Calculate pitch from note value.
        offset note by 57 halfnotes to get correct pitch from the range we have chosen for the notes.
    */
    double p = pow(DSP::chromatic_ratio, note - 57);
    p *= 440;
    return p;
}

void callback::audio_callback(void* unused, Uint8* byte_stream, int byte_stream_length) {

    /*
        This function is called whenever the audio buffer needs to be filled to allow
        for a continuous stream of audio.
        Write samples to byteStream according to byteStreamLength.
        The audio buffer is interleaved, meaning that both left and right channels exist in the same
        buffer.
    */

    // zero the buffer
    memset(byte_stream, 0, byte_stream_length);


    if (manager::get_instance()->quit) {
        return;
    }

    // cast buffer as 16bit signed int.
    Sint16* s_byte_stream = (Sint16*)byte_stream;

    // buffer is interleaved, so get the length of 1 channel.
    int remain = byte_stream_length / 2;

    // split the rendering up in chunks to make it buffersize agnostic.
    long chunk_size = 64;
    int iterations = remain / chunk_size;
    for (long i = 0; i < iterations; i++) {
        long begin = i * chunk_size;
        long end = (i * chunk_size) + chunk_size;
        manager::get_instance()->write_samples_to_buffer(s_byte_stream, begin, end, chunk_size);
    }
}

void manager::write_samples_to_buffer(int16_t* s_byteStream, long begin, long end, long length) {

    //////////////////////////////////////////////////////////////////////
    /*Sub all of this for basically summing the samples from the voices.*/
    //////////////////////////////////////////////////////////////////////
    if (samples == nullptr){
        samples = new int[length];
    }
    memset(samples, 0, length * sizeof(int));
    for (int i = 0; i < max_num_synths; i ++) {
        if (synths[i].active) {
            synths[i].evaluate_samples(length);
        }
    }

    for (int i = 0; i < length; i++){
        for (int s = 0; s < max_num_synths; s++){
            if (synths[s].active) {
                samples[i] += synths[s].sample[i];
            }
        }
        if (s_byteStream != NULL) {
            samples[i] *= 0.1; // scale master post sum volume.
            //myfile << std::to_string(samples[i]);
            //myfile << "\n";
            s_byteStream[i + begin] = (int16_t) samples[i]; // left channel
            s_byteStream[i + begin + 1] = (int16_t) samples[i]; // right channel
        }
    }
}


app_params manager::get_params() {
    app_params ap;
    ap.number_synths = this->max_num_synths;
    ap.sample_rate = this->sample_rate;
    return ap;
}



//set up
void manager::set_up(app_params ap) {
    this->sample_rate = ap.sample_rate;
    this->table_length = 1024;
    max_num_synths = ap.number_synths;
    for (int i = 0; i < 8; i++)
    {
        synths.push_back(Synth::Synth(sample_rate, table_length));
    }
    for (int i = 0; i < max_num_synths; i++)
    {
        for (int j = 0; j < ap.sps[i].polymax; j++) {
            ap.sps[i].vps[j].flagged = true;
            if (j >= 0) {
                ap.sps[i].vps[j].mode = 2;
            }
        }

        synths[i].init_synth(ap.sps[i]);
        //synths[i].flag();
    }
    setup_sdl();
    setup_sdl_audio();
}

void manager::setup_sdl(void) {

    SDL_Init(SDL_INIT_VIDEO);

    // Get current display mode of all displays.
    SDL_DisplayMode current;
    for (int i = 0; i < SDL_GetNumVideoDisplays(); ++i) {
        int should_be_zero = SDL_GetCurrentDisplayMode(i, &current);
        if (should_be_zero != 0) {
            // In case of error...
            if (debuglog) { SDL_Log("Could not get display mode for video display #%d: %s", i, SDL_GetError()); }
        }
        else {
            // On success, print the current display mode.
            if (debuglog) { SDL_Log("Display #%d: current display mode is %dx%dpx @ %dhz. \n", i, current.w, current.h, current.refresh_rate); }
        }
    }

    window = SDL_CreateWindow("", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_OPENGL);

    if (window != NULL) {
        context = SDL_GL_CreateContext(window);
        if (context == NULL) {
            printf("\nFailed to create context: %s\n", SDL_GetError());
        }

        renderer = SDL_CreateRenderer(window, -1, 0);
        if (renderer != NULL) {
            SDL_GL_SetSwapInterval(1);
            SDL_SetWindowTitle(window, "SDL2 synth sample 2");
        }
        else {
            if (debuglog) {
                printf("Failed to create renderer: %s", SDL_GetError());
            }
        }
    }
    else {
        if (debuglog) {
            printf("Failed to create window:%s", SDL_GetError());
        }
    }
}
int manager::setup_sdl_audio(void) {

    SDL_Init(SDL_INIT_AUDIO | SDL_INIT_TIMER);
    SDL_AudioSpec want;
    SDL_zero(want);
    SDL_zero(audio_spec);

    want.freq = sample_rate;
    // request 16bit signed little-endian sample format.
    want.format = AUDIO_S16LSB;
    // request 2 channels (stereo)
    want.channels = 2;
    want.samples = buffer_size;

    /*
        Tell SDL to call this function (audio_callback) that we have defined whenever there is an audiobuffer ready to be filled.
    */
    want.callback = callback::audio_callback;

    if (debuglog) {
        printf("\naudioSpec want\n");
        printf("----------------\n");
        printf("sample rate:%d\n", want.freq);
        printf("channels:%d\n", want.channels);
        printf("samples:%d\n", want.samples);
        printf("----------------\n\n");
    }

    audio_device = SDL_OpenAudioDevice(NULL, 0, &want, &audio_spec, 0);

    if (debuglog) {
        printf("\naudioSpec get\n");
        printf("----------------\n");
        printf("sample rate:%d\n", audio_spec.freq);
        printf("channels:%d\n", audio_spec.channels);
        printf("samples:%d\n", audio_spec.samples);
        printf("size:%d\n", audio_spec.size);
        printf("----------------\n");
    }

    if (audio_device == 0) {
        if (debuglog) { printf("\nFailed to open audio: %s\n", SDL_GetError()); }
        return 1;
    }

    if (audio_spec.format != want.format) {
        if (debuglog) { printf("\nCouldn't get requested audio format.\n"); }
        return 2;
    }

    buffer_size = audio_spec.samples;
    SDL_PauseAudioDevice(audio_device, 0);// unpause audio.
    return 0;
}

//clean up
void manager::clean_up() {
    cleanup_data();
    destroy_sdl();
}
void manager::cleanup_data(void) {
    
    //helper::free_memory(sine_wave_table);
    //helper::free_memory(saw_wave_table);
    //delete[] sine_wave_table;
    //delete[] saw_wave_table;
    //printf("alloc count:%d\n", helper::alloc_count);
}
void manager::destroy_sdl(void) {

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_CloseAudioDevice(audio_device);
    SDL_Quit();
}