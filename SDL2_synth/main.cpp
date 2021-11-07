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
	app_params ap = helper::load_dparams("new_params.json") ;

	//////////////////////////////////////////////////////////
	manager::get_instance()->set_up(ap);
	printf("setup complete\n");
	//manager::get_instance()->manager::run_synth();
	//manager::get_instance()->clean_up();


	//////////////////////////////////////////////////////////
	//rpc::server srv(8080);
	//srv.bind("reload", []() {
	//	manager::get_instance()->hot_load(helper::load_dparams("new_params.json"));
	//	});
	//srv.run();

	reciever rec = reciever(8080);
	reciever* p_rec = &rec;
	//rec.run();
	std::thread reciever_thread(&reciever::run, p_rec);
	printf("after threaded reciever\n");

	//auto future = std::async(std::launch::async, &manager::run_synth, manager::get_instance());

	while (!manager::get_instance()->quit) {
		manager::get_instance()->manager::main_loop(p_rec);
	}
	manager::get_instance()->clean_up();
	reciever_thread.join();

	return 0;
}

void reload_call() {
}


void start() {

}
