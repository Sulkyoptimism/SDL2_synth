#include "reciever.h"


reciever::~reciever()
{
}

void reciever::run()
{
	//rpc::server srv(8080);
	//srv.bind("reload", &hotload);
	//srv.run();
}

void reciever::reset()
{
	trigger_hotload = false;
}


void reciever::hotload()
{
	trigger_hotload = true;
}

bool reciever::get_hotload()
{
	return trigger_hotload;
}
