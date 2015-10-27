#pragma once

#include <cstddef>

class SmartRawMemory {
public:
    class ExceptionAlreadyAllocated {};
    class ExceptionNotAllocated {};
    class ExceptionOutOfMemory {};

	SmartRawMemory();

	~SmartRawMemory();

	void allocate(size_t size);

	void reallocate(size_t newSize);

	void free();

	void freeIfAllocated();

	void *getPtrSafe();

protected:
	void *pointer;
};
