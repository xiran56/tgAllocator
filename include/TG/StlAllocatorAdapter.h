#pragma once

#include <cstdint>
#include <cassert>
#include <new>

namespace memory {
    /* Класс для совместиммости с контейнерами STL */

    template<class T, class AllocationPolicy>
    class StlAllocatorAdapter {
        public:
            using value_type = T;
            using pointer = T*;
            using const_pointer = const T*;
            using reference = T&;
            using const_reference = const T&;
            using difference_type = ptrdiff_t;
            using size_type = size_t;

            template<class U, class AllocPolicy>
            friend class StlAllocatorAdapter;

            template<class U>
            struct rebind {
                using other = StlAllocatorAdapter<U, AllocationPolicy>;
            };

        public:
            StlAllocatorAdapter() noexcept = default;

            explicit StlAllocatorAdapter(AllocationPolicy *policy) noexcept : _policy { policy } {  }

            StlAllocatorAdapter(const StlAllocatorAdapter<T, AllocationPolicy> &other) noexcept : _policy { other._policy } {  }
       
            StlAllocatorAdapter(StlAllocatorAdapter<T, AllocationPolicy> &&other) noexcept : _policy { other._policy } {  }

            template<class U>
            StlAllocatorAdapter(const StlAllocatorAdapter<U, AllocationPolicy> &other) noexcept : _policy { other._policy } {  }

            template<class U>
            StlAllocatorAdapter(StlAllocatorAdapter<U, AllocationPolicy> &&other) noexcept : _policy { other._policy } {  }

            T* allocate(size_t objectsCount) noexcept {
                assert(this->_policy && "Uninitialized allocation policy!");

                return static_cast<T*>(this->_policy->allocate(objectsCount, alignof(T)));
            }

            void deallocate(void *address, size_t objectsCount) noexcept {
                assert(this->_policy && "Uninitialized allocation policy!");

                union {
                    void *currentFreeAddressAsVoidPtr = address;

                    uintptr_t currentFreeAddressAsUintPtr;
                };

                for (auto i { 0u }; i < objectsCount; ++i) {
                    currentFreeAddressAsUintPtr += sizeof(T) * i;

                    this->_policy->free(currentFreeAddressAsVoidPtr);
                }
            }

            template<class U, class... Args>
            U* construct(U *ptr, Args &&...args) {
                return new (reinterpret_cast<void*>(ptr)) U(args...);
            }

            template<class U>
            void destroy(U *ptr) {
                ptr->~U();
            }

        private:
            AllocationPolicy *_policy { nullptr };
    };
}
