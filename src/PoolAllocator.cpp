#include "TG/PoolAllocator.h"
#include "TG/adjustment.h"
#include <cmath>
#include <cassert>

memory::PoolAllocator::PoolAllocator(void *address, size_t size, const size_t objectSize, const uint8_t objectAlignment) : AllocatorBaseImpl(address, size),
    _objectSize { objectSize }, _objectAlignment { objectAlignment } {
        assert(this->_objectSize != 0 && "objectSize is 0!");

        this->clear();
}

memory::PoolAllocator::~PoolAllocator() {
    this->_freeList = nullptr;
}

void* memory::PoolAllocator::allocate(size_t n, uint8_t alignment) {
    assert(n == this->_objectSize && "n isn't equals to allocator's object size!");

    assert(alignment == this->_objectAlignment && "alignment isn't equals to allocator's object alignment!");

    /* Если кончилась память */

    if (!this->_freeList)
        return nullptr;

    void *ptr = this->_freeList;

    this->_freeList = reinterpret_cast<void**>(*this->_freeList);

    this->_offset += this->_objectSize;

    return ptr;
}

void memory::PoolAllocator::free(void *address) {
    *(reinterpret_cast<void**>(address)) = this->_freeList;

    this->_freeList = reinterpret_cast<void**>(address);

    this->_offset -= this->_objectSize;
}

void memory::PoolAllocator::clear() {
    const auto adjustment = alignForwardAdjustment(this->_begin, this->_objectAlignment);

    const auto objectsCount = static_cast<size_t>(std::floor((this->_size - adjustment) / this->_objectSize));

    union {
        void *asVoidPtr;

        uintptr_t asUintPtr;
    };

    asVoidPtr = this->_begin;

    asUintPtr += adjustment;

    this->_freeList = reinterpret_cast<void**>(asVoidPtr);

    auto ptr = this->_freeList;

    for (auto i { 0 }; i < (objectsCount - 1); ++i) {
        *ptr = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(ptr) + this->_objectSize);

        ptr = reinterpret_cast<void**>(*ptr);
    }

    *ptr = nullptr;
}
