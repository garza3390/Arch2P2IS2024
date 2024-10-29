#ifndef RAM_H
#define RAM_H

#include <array>
#include <cstdint>

struct RAM {
    std::array<uint64_t, 256> memory;  // Memoria del sistema con 256 posiciones de 64 bits
};


#endif