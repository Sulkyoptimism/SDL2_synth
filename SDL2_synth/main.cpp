//#define SDL_MAIN_HANDLED
#include "SDL2/SDL.h"
#include <thread>
#include <future>

#include "manager.h"
#include "inc/rpc/server.h"


int main(int argc, char* args[]) {
	SDL_SetMainReady();

	helper::export_dparams();
	double sample_rate = 44100;
	app_params ap = helper::load_dparams("synthout.json") ;

	//////////////////////////////////////////////////////////
	manager::get_instance()->set_up(ap);
	printf("setup complete\n");


	receiver rec = receiver(8080);
	receiver* p_rec = &rec;
	std::thread receiver_thread(&receiver::run, p_rec);
	printf("after threaded receiver\n");

	while (!manager::get_instance()->quit) {
		manager::get_instance()->manager::main_loop(p_rec);
	}
	manager::get_instance()->clean_up();
	receiver_thread.join();

	return 0;
}
