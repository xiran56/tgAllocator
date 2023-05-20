#include "TG/AllocatorBaseImpl.h"

memory::AllocatorBaseImpl::AllocatorBaseImpl(void *address, size_t size) : _begin(address), _size(size), _offset(0) {
	
}

memory::AllocatorBaseImpl::~AllocatorBaseImpl() {
	if (!this->_offset) {
		this->_size = 0;
		
		this->_begin = nullptr;
	}
}
