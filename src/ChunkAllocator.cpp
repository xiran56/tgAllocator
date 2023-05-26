#include "TG/ChunkAllocator.h"
#include "TG/Chunk.h"
#include <cassert>

memory::ChunkAllocator::ChunkAllocator(size_t chunksPerBlock, PoolAllocator *pool) : _chunksPerBlock { chunksPerBlock }, _pool { pool } {

}

memory::ChunkAllocator::~ChunkAllocator() {

}

memory::Chunk* memory::ChunkAllocator::allocateBlock(size_t chunkSize) {
    auto blockSize = this->_chunksPerBlock * chunkSize;

    assert(blockSize == this->_pool->_objectSize && "Size of block to allocate isn't equals to pool's block size!");

    auto blockBegin = reinterpret_cast<Chunk*>(this->_pool->allocate(this->_pool->_objectSize, this->_pool->_objectAlignment));

    Chunk *currentChunk { blockBegin };

    for (auto i { 0u }; i < this->_chunksPerBlock - 1; ++i) {
        currentChunk->next = reinterpret_cast<Chunk*>(reinterpret_cast<char*>(currentChunk) + chunkSize); 

        currentChunk = currentChunk->next;
    }

    currentChunk->next = nullptr;

    return currentChunk;
}

void* memory::ChunkAllocator::allocate(size_t n, size_t alignment) {
    if (!this->_head)
        this->_head = this->allocateBlock(n);

    auto ptr = this->_head;

    this->_head = this->_head->next;

    return ptr;
}

void memory::ChunkAllocator::free(void *address, size_t size) {
}
