#include "receiver.h"

receiver::receiver(int port)
{
	this->port = port;
}

void receiver::hotload()
{
	trigger_hotload = true;
	printf("Hot load bool called\n");
}

void receiver::run()
{
	rpc::server srv(port);
	printf("server started\n");
	//srv.bind("reload", &receiver::hotload);
	srv.bind("reload", [this]() {
		trigger_hotload = true;
		printf("Hot load bool called\n");
		}
	);	
	srv.bind("addnotes", [this](std::vector<std::pair<int,int>> notes_info) {
		note_ready = true;
		syn_notes = notes_info;
		printf("new note int/bool called\n");
		}
	);
	printf("server about to run\n");

	srv.run();
	printf("server running\n");

}

void receiver::reset_hotload()
{
	trigger_hotload = false;
}

void receiver::reset_note()
{
	note_ready = false;
	syn_notes.clear();
}

bool receiver::get_hotload()
{
	return trigger_hotload;
}

bool receiver::get_note_ready()
{
	return note_ready;
}

std::vector<std::pair<int,int>> receiver::get_next_note()
{
	return syn_notes;
}
