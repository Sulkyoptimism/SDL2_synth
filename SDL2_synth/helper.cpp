#include "helper.h"
int helper::alloc_count = 0;
bool helper::debuglog = false;

helper::helper()
{

}

helper::~helper()
{
}


void* helper::alloc_memory(size_t size, const char* name) {

    void* ptr = nullptr;
    ptr = malloc(size);
    if (ptr == NULL) {
        //if (debuglog) {
        //    printf("alloc_memory error: malloc with size %zu returned NULL.\n name:%s", size, name);
        //}
    }
    else {
        alloc_count++;
    }
    return ptr;
}

void* helper::free_memory(void* ptr) {

    if (ptr != NULL) {
        free(ptr);
        alloc_count--;
    }
    return NULL;
}


