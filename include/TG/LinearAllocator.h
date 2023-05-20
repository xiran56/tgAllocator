#pragma once

#include "AllocatorBaseImpl.h"
#include <cassert>
#include <cstdint>

namespace memory {
	/* Класс линейного аллокатора */
	
	class LinearAllocator : protected AllocatorBaseImpl {
		public:
			LinearAllocator(void *address, size_t size);
			
			~LinearAllocator();
			
			void* allocate(size_t size, uint8_t alignment);
			
			void clear();
	};
	
	template<class T>
	inline void deleteObject(LinearAllocator *allocator, T *object) {
		assert(false && "LinearAllocator does not support free() (which deleteObject calls). Instead you can call destructor manually (for ALL allocated objects) and call clear() (deletes ALL allocated objects)");
	}
}
