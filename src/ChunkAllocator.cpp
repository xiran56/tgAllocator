#include "TG/ChunkAllocator.h"
#include <cassert>

memory::ChunkAllocator::ChunkAllocator(size_t chunksPerBlock, LinearAllocator *alloc) : _chunksPerBlock { chunksPerBlock }, _alloc { alloc }, _currentPtr { nullptr } {

}

memory::ChunkAllocator::~ChunkAllocator() {

}

memory::Chunk* memory::ChunkAllocator::allocateBlock(size_t size) {
    auto blockSize = this->_chunksPerBlock * size;
    
    auto blockBegin = reinterpret_cast<Chunk*>(this->_alloc->allocate(blockSize, 0)); 

    auto currentChunk = blockBegin;

    for (auto i { 0u }; i < this->_chunksPerBlock - 1; ++i) {
        currentChunk->next = reinterpret_cast<Chunk*>(reinterpret_cast<char*>(currentChunk) + size);

        currentChunk = currentChunk->next;
    }

    currentChunk->next = nullptr;

    return blockBegin;
}

void* memory::ChunkAllocator::allocate(size_t n, size_t alignment) {
    if (!this->_currentPtr)
        this->_currentPtr = this->allocateBlock(n);

    auto freeChunk = this->_currentPtr;

    this->_currentPtr = this->_currentPtr->next;

    return freeChunk;
}

void memory::ChunkAllocator::free(void *address) {
    reinterpret_cast<Chunk*>(address)->next = this->_currentPtr;

    this->_currentPtr = reinterpret_cast<Chunk*>(address);
}

void memory::ChunkAllocator::clear() {
    this->_alloc->clear();
}
