#pragma once
#include <stdint.h>
#include <math.h>
enum synth_mode { SINE = 0, SQUARE = 1, TRI = 2, SAW = 3 };
namespace DSP {
	const  double pi = 3.14159265358979323846;
	const  double chromatic_ratio = 1.059463094359295264562;
}


class wavetables
{
public:

	static wavetables* get_instance();
	bool init_called = false;

	double sample_rate= 1;
	int table_length = 1;

	int16_t* sine_wave_table;
	int16_t* saw_wave_table;

	void init(double sample_rate, int table_length);

	int16_t get_sample_from_table(int phase_int, int synthwave_mode, float pulse_width);

	void build_sine_table(int16_t* data, int wave_length);
	void build_saw_table(int16_t* data, int wave_length);
	int16_t square_from_sine(int index, float pulse_width);
	int16_t triangle_from_sin(int index);
private:
	static wavetables* singleton_instance;
	wavetables();

};

