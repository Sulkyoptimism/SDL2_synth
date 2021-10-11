#include "SDL2/SDL.h"
#include "manager.h"

int main(int argc, char* args[]) {
	double sample_rate = 44100;
	manager::get_instance()->set_up(sample_rate);
	while (!manager::get_instance()->quit)
	{
		manager::get_instance()->main_loop();
	}
	manager::get_instance()->clean_up();
	return 0;
}