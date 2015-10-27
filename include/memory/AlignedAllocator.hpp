#pragma once

#include "memory/SmartRawMemory.h"

// TODO< throw out the access >

// this is a template for a class that Allocate Align Data blocks of one type
// AlignSize : is the number in bytes of the align size
// Type      : is the to align Type
template<unsigned long AlignSize, typename Type>
class AlignedAllocator {
	static const unsigned long batchSize = 64; // how much do we additionaly allocate if no more space is there?
    static const unsigned long Mask = -1 ^ (AlignSize-1);

public:
	// gets thrown if the Position of the Memory was changed
    struct ExceptionMemoryPointerLocked {};
	
	// can throw the class NoMemory
	AlignedAllocator() {
		allocSize = batchSize;
		mpLocked = false;

		// calculate the alignedSize
		ElementSize = ((sizeof(Type) + AlignSize) / AlignSize) * AlignSize;

		rawMemory.allocate(ElementSize * allocSize + AlignSize * 2);

        alignedMemory = reinterpret_cast<void*>((reinterpret_cast<unsigned long>(rawMemory.getPtrSafe()) & Mask) + AlignSize);
	}

    ~AlignedAllocator() {
    }

	// locks the memory, so it don't allow allocations that change the pointers (is a allocation with a different memory pointer)
	void setLock(bool locked = true) {
		mpLocked = locked;
	}
	
	// this reallocate the memory that you can use up to "Count" Elements without a reallocation
	// sets the acutal Ptr to the begin

	// throws MemoryPointerLocked if the Memory position has changed
	void reallocateElements(unsigned long Count) {
        if( mpLocked ) {
            throw AlignedAllocator::ExceptionMemoryPointerLocked();
        }

        rawMemory.reallocate(ElementSize * (Count) + AlignSize * 2);
		allocSize = Count;

        void* alignedMemory2 = reinterpret_cast<void*>( (reinterpret_cast<unsigned long>(rawMemory.getPtrSafe()) & Mask) + AlignSize);
		
		if( alignedMemory2 != alignedMemory ) {
			alignedMemory = alignedMemory2;


		}
	}

	void resizeIfRequiredTo(unsigned int requiredSize) {
		if( allocSize < requiredSize ) {
			reallocateElements(requiredSize);
		}
	}

	// is a overloaded operator that returns the pointer to an element
	// is a "bit" slower than the iterator thing
	Type* operator[](unsigned long Index) {
		// TODO< check the Index with my own assert thing, maybe with a template parameter? as a "check option"? >

		return reinterpret_cast<Type*>(reinterpret_cast<unsigned long>(alignedMemory) + ElementSize * Index);
	}

	unsigned getAllocatedSize() const {
		return allocSize;
	}

	void *getAlignedPtr() const {
		return alignedMemory;
	}

   private:
	SmartRawMemory rawMemory;
    void* alignedMemory;
	
    unsigned long allocSize;   // is the size that was got allocated
	unsigned long ElementSize; // is the size in bytes of an aligned Element

	bool mpLocked; // is the memory-pointer locked (means that a reallocation can't change the pointer, if it does the AllocateElement member throws a MemoryPointerLocked exception)
	               // Tip: just use the reallocateElements member to allocate enougth memory before you use it
};
