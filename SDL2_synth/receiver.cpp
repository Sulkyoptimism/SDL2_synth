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
	srv.bind("addnotes", [this](int notes_count, std::pair<int,int> notes_info) {
		notes_in_count = notes_count;
		note_ready = true;
		syn_notes.push(notes_info);
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
	if (notes_in_count - 1 < 1) {
		note_ready = false;
		std::queue<std::pair<int,int>> empty;
		syn_notes.swap(empty);
	}
	notes_in_count--;
}

bool receiver::get_hotload()
{
	return trigger_hotload;
}

bool receiver::get_note_ready()
{
	return note_ready;
}

std::queue<std::pair<int,int>> receiver::get_next_note()
{
	return syn_notes;
}
