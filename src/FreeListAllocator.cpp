#pragma once

#include "TG/FreeListAllocator.h"
#include "TG/AllocatorBaseImpl.h"
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

        /* Если размера текущего блока не хватает, пробуем следующий */
            
        if (freeBlock.asPtr->size < totalSize) {
            prevFreeBlock = freeBlock.asPtr;

            freeBlock.asPtr = freeBlock.asPtr->next;

            continue;
        }

        /* Тоже самое */

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

        auto header = reinterpret_cast<AllocationHeader*>(freeBlock.asUintPtr - sizeof(AllocationHeader));

        header->size = n;

        header->adjustment = adjustment;

        this->_size += totalSize;
    
        return reinterpret_cast<void*>(freeBlock.asPtr);
    }

    return nullptr;
}

void memory::FreeListAllocator::free(void *address) {
    assert(address && "Forwarded nullptr!");

    union {
        void *asVoidPtr;

        uintptr_t asUintPtr;
    };

    asVoidPtr = address;

    auto header = reinterpret_cast<AllocationHeader*>(asUintPtr - sizeof(AllocationHeader));
    
    union {
        FreeBlock* blockAsFreeBlock;
        
        uintptr_t blockAsUintPtr;
    };    

    blockAsUintPtr = asUintPtr - header->adjustment;

    auto blockSize = header->size;

    auto blockEnd = blockAsUintPtr + blockSize;

    auto freeList = this->_head;

    auto prevFreeList = nullptr;

    
}
