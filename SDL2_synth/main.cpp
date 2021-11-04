#define SDL_MAIN_HANDLED
#include "SDL2/SDL.h"
#include <thread>
#include "manager.h"

int main(int argc, char* args[]) {
	SDL_SetMainReady();

	helper::export_dparams();
	double sample_rate = 44100;
	app_params ap = helper::load_dparams("new_params.json") ;

	rpc::server srv(8080);
	srv.bind("reload", []() {
		manager::get_instance()->hot_load(helper::load_dparams("new_params.json"));
		});
	
	std::thread reciever_thread(&rpc::server::run, srv);

	manager::get_instance()->set_up(ap);
	while (!manager::get_instance()->quit)
	{
		manager::get_instance()->main_loop();
	}
	manager::get_instance()->clean_up();
	return 0;
}

void reload_call() {
}

void server_start() {

}
