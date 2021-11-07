#include "reciever.h"




reciever::reciever(int port)
{
	this->port = port;
}



void reciever::hotload()
{
	trigger_hotload = true;
	printf("Hot load bool called\n");
}

void reciever::run()
{
	rpc::server srv(8080);
	printf("server started\n");
	//srv.bind("reload", &reciever::hotload);
	srv.bind("reload", [this]() {
		trigger_hotload = true;
		printf("Hot load bool called\n");
		}
	);
	printf("server about to run\n");

	srv.run();
	printf("server running\n");

}

void reciever::reset()
{
	trigger_hotload = false;
}


bool reciever::get_hotload()
{
	return trigger_hotload;
}
