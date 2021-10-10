#include "Synth.h"
#include "manager.h"

Synth::Synth(voice_params vp){
	active = false;
	sample = nullptr;

	for (int i = 0; i < 8; i++) {
		voices.push_back(voice(sample_rate, vp, table_length));
	}
}



void Synth::synth_init(int id){
	active = true;
	this->id = id;
}

Synth::~Synth()
{
}


/// returns 0 for okay assignment, 1 for same note picked, -1 for no voices left.
int Synth::assign_newnote(int new_note) {
	for (int i = 0; i < 8; i++){
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
			return 0;
		}
		
	}
	return -1;
	//err no voices left
}



int* Synth::evaluate_samples(){
	if (sample == nullptr){
		sample = new int[block_size];
	}
	memset(sample, 0, block_size * sizeof(int));
	for (int v = 0; v < 8; v++) {
		if (voices[v].active){
			voices[v].write_samples(block_size);
			for (int i = 0; i < block_size; i++){
				sample[i] += voices[v].sample[i];
				//manager::get_instance()->myfile << std::to_string(sample[i]) << "\n";

			}
		}
		//printf("dad?");
	}
	return sample;
}