#pragma once
#include "inc/rpc/server.h"

class receiver
{
private:
	int port = 0;
	bool quit = false;
	bool trigger_hotload = false;
	bool note_ready = false;
	std::vector<std::pair<int, int>> syn_notes;
public:
	receiver(int port);
	void run();
	void reset_hotload();
	void reset_note();
	void hotload();
	bool get_hotload();
	bool get_note_ready();
	std::vector<std::pair<int, int>> get_next_note();


};

