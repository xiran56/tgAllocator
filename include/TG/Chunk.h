#pragma once

#include <cstdint>

namespace memory {
    /* Чанк одноименного аллокатора */

    struct Chunk {
        Chunk *next;

        uint32_t size;
    };
}
