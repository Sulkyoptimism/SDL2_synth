#pragma once
#include "inc/rpc/server.h"

class reciever
{
private:
	rpc::server srv;
	bool trigger_hotload = false;
public:
	reciever();
	~reciever();
	void run();
	void reset();
	void hotload();
	bool get_hotload();


};

