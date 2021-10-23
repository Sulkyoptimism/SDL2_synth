#include "Synth.h"

//synth_params Synth::default_params = {
//	-1,
//	true,
//	{voice::default_params,
//	voice::default_params,
//	voice::default_params,
//	voice::default_params,
//	voice::default_params,
//	voice::default_params,
//	voice::default_params,
//	voice::default_params}
//
//};

Synth::Synth(double sample_rate, int table_length){
	active = false;
	sample = nullptr;
	this->sample_rate = sample_rate;
	this->table_length = table_length;
	synth_params test_params = default_params;
	for (int i = 0; i < 8; i++) {
		voices.push_back(voice(sample_rate, table_length));
	}

}

void Synth::init_synth(synth_params sp) {
	id = sp.id;
	poly_mode = sp.poly_mode;
	for (int i = 0; i < 8; i++) {
		voices[i].init_voice(id, sp.vps[i]);
	}
}

void Synth::key_press(int note, bool b) {
	if (poly_mode) {
		for (int i = 0; i < 8; i++) {
			if (voices[i].active) {
				voices[i].key_press(note, b, !poly_mode);
			}
		}
	}
	else {
		for (int i = 0; i < 8; i++) {
			if (voices[i].flagged) {
				voices[i].key_press(note, b, !poly_mode);
			}
		}
	}
}

void Synth::synth_activate(int id){
	active = true;
	this->id = id;
}

Synth::~Synth()
{
}

void Synth::clean_up() {
	voice::delete_data();

}

/// returns 0 for okay assignment, 1 for same note picked, -1 for no voices left.
int Synth::assign_newnote(int new_note) {
	if (poly_mode) {
		for (int i = 0; i < 8; i++) {
			if (voices[i].active) {
				if (voices[i].note == new_note) {
					return 1;
				}
			}
			else {
				//new voice new note assignment
				voices[i].active = true;
				voices[i].id = i;
				voices[i].note = new_note;
				voices[i].key_pressed = true;
				return 0;
			}
		}
		return -1;
	}
	else {
		for (int i = 0; i < 8; i++) {
			if (voices[i].flagged) {
				voices[i].active = true;
				voices[i].id = i;
				voices[i].note = new_note;
				voices[i].key_pressed = true;
			}
		}
	}
	//err no voices left
}



int* Synth::evaluate_samples(int block_size){
	if (sample == nullptr){
		sample = new int[block_size];
	}
	memset(sample, 0, block_size * sizeof(int));
	for (int v = 0; v < 8; v++) {
		if (voices[v].active){
			voices[v].write_samples(block_size);
			for (int i = 0; i < block_size; i++){
				sample[i] += voices[v].sample[i];
			}
		}
	}
	return sample;
}