#include "SDL2/SDL.h"
#include "manager.h"

int main(int argc, char* args[]) {
	helper::export_dparams();
	double sample_rate = 44100;
	app_params ap = helper::load_dparams("default_params.json") ;

	manager::get_instance()->set_up(ap);
	while (!manager::get_instance()->quit)
	{
		manager::get_instance()->main_loop();
	}
	manager::get_instance()->clean_up();
	return 0;
}