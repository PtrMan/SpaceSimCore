#pragma once

template<unsigned int StaticAllocatorSize, typename Type>
struct FixedAllocator {
	Type &operator[](int index) {
		return fixedArray[index];
	}

	FixedAllocator() {}
protected:	
	Type fixedArray[StaticAllocatorSize];
};