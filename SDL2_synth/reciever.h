#pragma once
#include "inc/rpc/server.h"

class reciever
{
private:
	int port = 0;
	bool quit = false;
	bool trigger_hotload = false;
public:
	reciever(int port);
	void bind();
	void run();
	void reset();
	void hotload();
	bool get_hotload();


};

