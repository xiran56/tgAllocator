#pragma once

#include <cstddef>

namespace memory {
    /* Базовый класс аллокатора */
	
	class AllocatorBaseImpl {
		protected:
			AllocatorBaseImpl(void *address, size_t size);
			
			~AllocatorBaseImpl();
			
			void *_begin;
			
			size_t _size;
			
			size_t _offset;
	};
}
