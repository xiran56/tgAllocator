#pragma once

#include "AllocatorBaseImpl.h"
#include <cstdint>

namespace memory {
	/* Класс стекового аллокатора */
	
	class StackAllocator : protected AllocatorBaseImpl {
		public:
			StackAllocator(void *address, size_t size);
			
			~StackAllocator();
			
			void* allocate(size_t n, uint8_t alignment);
			
			void free(void *address);
			
			void clear();
	};
}
