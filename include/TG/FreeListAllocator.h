#pragma once

#include <cstdint>
#include "AllocatorBaseImpl.h"

namespace memory {
    /* Класс спискового аллокатора */

    class FreeListAllocator : public AllocatorBaseImpl {
        public:
            FreeListAllocator(void *address, size_t size);

            ~FreeListAllocator();

            void* allocate(size_t n, uint8_t alignment);

            void free(void *address);

            void clear();

        private:
            struct AllocationHeader {
                size_t size;

                uint8_t adjustment;
            };

            struct FreeBlock {
                size_t size;

                FreeBlock *next { nullptr };
            };

            FreeBlock *_head;
    };
}
