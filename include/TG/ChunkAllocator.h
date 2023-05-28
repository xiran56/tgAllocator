#pragma once

#include "Chunk.h"
#include "LinearAllocator.h"

namespace memory {
    /* Класс чанкового аллокатора */

    class ChunkAllocator {
        public:
            ChunkAllocator(size_t chunksPerBlock, LinearAllocator *alloc);

            ~ChunkAllocator();

            void* allocate(size_t n, size_t alignment);

            void free(void *address);

            void clear();

        private:
            size_t _chunksPerBlock;

            LinearAllocator *_alloc;

            Chunk *_currentPtr;

            Chunk* allocateBlock(size_t chunkSize);
    };
}
