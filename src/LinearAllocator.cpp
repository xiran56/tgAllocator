#include "TG/LinearAllocator.h"
#include "TG/adjustment.h"
#include <cassert>

memory::LinearAllocator::LinearAllocator(void *address, size_t size) : AllocatorBaseImpl(address, size) {
	
}

memory::LinearAllocator::~LinearAllocator() {
	
}

void* memory::LinearAllocator::allocate(size_t n, uint8_t alignment) {
	assert(n && "memory::LinearAllocator::allocate(n, alignment) called with n = 0");
	
	union {
		void *usedAsVoidPtr;
		
		uintptr_t usedAsUintPtr;
	};
	
	/* Сдвигаем указатель */
	
	usedAsVoidPtr = this->_begin;
	
	usedAsUintPtr += this->_offset;
	
	/* Получаем корректировку */
	
	auto adjustment = alignForwardAdjustment(usedAsVoidPtr, alignment);
	
	/* Если кончилась память */
	
	if (this->_offset + n + adjustment > this->_size)
		return nullptr;
	
	usedAsUintPtr += adjustment;
	
	this->_offset += n + adjustment;
	
	return usedAsVoidPtr;
}

void memory::LinearAllocator::clear() {
	this->_offset = 0;
}
