#include "TG/adjustment.h"

uint8_t memory::alignForwardAdjustment(void *address, uint8_t alignment, uint8_t extra) {
	auto adjustment = alignForwardAdjustment(address, alignment);
	
	auto neededSpace = extra;
	
	if (adjustment < neededSpace) {
		neededSpace -= adjustment;
		
		adjustment += alignment * (neededSpace / alignment);
		
		if (neededSpace % alignment > 0)
			adjustment += alignment;
	}
	
	return adjustment;
}
