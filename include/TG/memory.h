#pragma once

#include <new>
#include <cstdint>

namespace memory {
	/* Создает объект */
	
	template<class T, class Allocator, class... Args>
	inline T* createObject(Allocator *allocator, Args&&... args) {
		void *mem = allocator->allocate(sizeof(T), alignof(T));
		
		return mem ? new (mem) T(args...) : nullptr;
	}
	
    /* Перемещает stackObject в новый объект */

    template<class T, class Allocator>
    inline T* createObject(Allocator *allocator, T &&stackObject) {
        void *mem = allocator->allocate(sizeof(T), alignof(T));

        return mem ? new (mem) T(stackObject) : nullptr;
    }

	/* Удаляет объект */
	
	template<class T, class Allocator>
	inline void deleteObject(Allocator *allocator, T *object) {
		object->~T();
		
		allocator->free(object);
	}

    inline auto kilobytesToBytes(uint32_t kilobytes) {           
        return (kilobytes * 1024);                               
    }                                                            
                                                             
    inline auto megabytesToKilobytes(uint32_t megabytes) {       
        return (megabytes * 1024);                               
    }                                                            
                                                             
    inline auto megabytesToBytes(uint32_t megabytes) {           
        return kilobytesToBytes(megabytesToKilobytes(megabytes));
    }                                                            
}
