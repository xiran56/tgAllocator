#pragma once

#include "TG/FreeListAllocator.h"
#include "TG/adjustment.h"
#include <cassert>

memory::FreeListAllocator::FreeListAllocator(void *address, size_t size) : AllocatorBaseImpl { address, size }, _head { reinterpret_cast<FreeBlock*>(address) } {
    assert(size < sizeof(FreeBlock) && "Size of address is lower then metainfo size!");

    this->_head->size = size;
}

memory::FreeListAllocator::~FreeListAllocator() {
    this->_head = nullptr;
}

void* memory::FreeListAllocator::allocate(size_t n, uint8_t alignment) {
    assert(n && "Size of block to allocate is 0!");
    
    FreeBlock *prevFreeBlock { nullptr };

    union {
        FreeBlock *asPtr;
        
        uintptr_t asUintPtr;
    } freeBlock;

    freeBlock.asPtr = this->_head;

    while (freeBlock.asPtr) {
        auto adjustment = alignForwardAdjustment(freeBlock.asPtr, alignment, sizeof(AllocationHeader));

        auto totalSize = n + adjustment;

        if (freeBlock.asPtr->size < totalSize) {
            prevFreeBlock = freeBlock.asPtr;

            freeBlock.asPtr = nullptr;

            continue;
        }

        if (freeBlock.asPtr->size - totalSize <= sizeof(AllocationHeader)) {
            totalSize = freeBlock.asPtr->size;

            if (prevFreeBlock) 
                prevFreeBlock->next = freeBlock.asPtr->next;
            else
                this->_head = freeBlock.asPtr->next;
        } else {
            auto nextBlock = reinterpret_cast<FreeBlock*>(freeBlock.asUintPtr + totalSize);

            nextBlock->size = freeBlock.asPtr->size - totalSize;

            nextBlock->next = freeBlock.asPtr->next;

            if (prevFreeBlock)
                prevFreeBlock->next = freeBlock.asPtr;
            else
                this->_head = nextBlock;
        }

        freeBlock.asUintPtr += adjustment;


    }

    return nullptr;
}
