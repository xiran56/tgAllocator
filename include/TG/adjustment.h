#pragma once

#include <cstdint>

namespace memory {
	/* Возвращает выровненный адрес */
	
	inline void* alignForward(void *address, uint8_t alignment) {
		return reinterpret_cast<void*>(
			(reinterpret_cast<uintptr_t>(address) + static_cast<uintptr_t>(alignment - 1)) & static_cast<uintptr_t>(~(alignment - 1))
		);
	}
	
	/* Возвращает корректировку адреса, необходимую для выравнивания */
	
	inline uint8_t alignForwardAdjustment(void *address, uint8_t alignment) {
		uint8_t adjustment = alignment - (reinterpret_cast<uintptr_t>(address) & static_cast<uintptr_t>(alignment - 1));
		
		return (adjustment == alignment) ? 0 : adjustment;
	}
	
	/* Возвращает корректировку адреса, необходимую для выравнивания + дополнительная корректировка */
	
	uint8_t alignForwardAdjustment(void *address, uint8_t alignment, uint8_t extra);
}