#include <stdlib.h>

#include "memory/SmartRawMemory.h"

SmartRawMemory::SmartRawMemory() {
	pointer = nullptr;
}

SmartRawMemory::~SmartRawMemory() {
	freeIfAllocated();
}

void SmartRawMemory::allocate(size_t size) {
    reallocate(size);
}

void SmartRawMemory::reallocate(size_t newSize) {
	void *calledPointer = realloc(pointer, newSize);
	if( calledPointer == nullptr ) {
        throw ExceptionOutOfMemory();
	}

	pointer = calledPointer;
}

void SmartRawMemory::free() {
	if( pointer == nullptr ) {
        throw ExceptionNotAllocated();
	}

	::free(pointer);

	pointer = nullptr;
}

void SmartRawMemory::freeIfAllocated() {
	if( pointer != nullptr ) {
		free();
	}
}


void *SmartRawMemory::getPtrSafe() {
	if( pointer == nullptr ) {
        throw ExceptionNotAllocated();
	}

	return pointer;
}
