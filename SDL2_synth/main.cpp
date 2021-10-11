#include "SDL2/SDL.h"
#include "manager.h"

int main(int argc, char* args[]) {
	manager::get_instance()->set_up(44100, 1024);
	while (!manager::get_instance()->quit)
	{
		manager::get_instance()->main_loop();
	}
	manager::get_instance()->clean_up();
	return 0;
}