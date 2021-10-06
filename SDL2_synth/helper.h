#include <stdio.h>
#include <stdlib.h>
#include "SDL2/SDL.h"

class helper
{
public:

    static int alloc_count;
    static bool debuglog;
    static void* alloc_memory(size_t size, const char* name);
    static void* free_memory(void* ptr);

	helper();
	~helper();

private:

};
