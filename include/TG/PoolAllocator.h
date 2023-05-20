#pragma once

#include "AllocatorBaseImpl.h"
#include <cstdint>

namespace memory {
    /* Класс пулового аллокатора */

    class PoolAllocator : protected AllocatorBaseImpl {
        public:
            PoolAllocator(void *address, size_t size, const size_t objectSize, const uint8_t objectAlignment);

            ~PoolAllocator();

            void* allocate(size_t n, uint8_t alignment);

            void free(void *address);

            void clear();

        private:
            const size_t _objectSize;

            const uint8_t _objectAlignment;

            void **_freeList;
    };
}
