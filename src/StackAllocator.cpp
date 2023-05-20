#include "TG/StackAllocator.h"
#include "TG/adjustment.h"
#include <cassert>

struct AllocationHeader {
    uint8_t adjustment;  
};

memory::StackAllocator::StackAllocator(void *address, size_t size) : AllocatorBaseImpl(address, size) {
	
}

memory::StackAllocator::~StackAllocator() {
	
}

void* memory::StackAllocator::allocate(size_t n, uint8_t alignment) {
	assert(n && "memory::StackAllocator::allocate(n, alignment) called with n = 0");
	
	union {
		void *usedAsVoidPtr;
		
		uintptr_t usedAsUintPtr;
		
		AllocationHeader *usedAllocationHeader;
	};
	
	usedAsVoidPtr = this->_begin;
	
	usedAsUintPtr += this->_offset;
	
	auto adjustment = alignForwardAdjustment(usedAsVoidPtr, alignment, sizeof(AllocationHeader));
	
	if (this->_offset + n + adjustment > this->_size)
		return nullptr;
	
	usedAllocationHeader->adjustment = adjustment;
	
	usedAsUintPtr += adjustment;
	
	this->_offset += n + adjustment;
	
	return usedAsVoidPtr;
}

void memory::StackAllocator::free(void *address) {
	union {
		void *addressAsVoidPtr;
		
		uintptr_t addressAsUintPtr;
		
		AllocationHeader *addressAllocationHeader;
	};
	
	addressAsVoidPtr = address;
	
	addressAsUintPtr -= sizeof(AllocationHeader);
	
	this->_offset -= (reinterpret_cast<uintptr_t>(this->_begin) + this->_offset) - (reinterpret_cast<uintptr_t>(address) + addressAllocationHeader->adjustment);
}

void memory::StackAllocator::clear() {
	this->_offset = 0;
}
