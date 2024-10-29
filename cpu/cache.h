#ifndef CACHE_H
#define CACHE_H

//#include <cstdint>
#include <array>
#include <string>
#include <iostream>
#include "ram.h" 
#include "bus.h"

struct cache {
    std::array<uint64_t, 8> data;             // 8 bloques de datos
    std::array<uint64_t, 8> addresses;        // Direcciones correspondientes
    std::array<std::string, 8> moesi_state;   // Estados MOESI por bloque
    bool bus_access_enabled = true;           // Acceso habilitado al bus
    int cache_misses = 0;
    int invalidations = 0;

    // Constructor
    cache();

    void load(int block, uint64_t addr, RAM &memory, bus &system_bus);

    void store(int block, uint64_t addr, RAM &memory, bus &system_bus);

    // Función para imprimir el estado de cada bloque en la cache
    void print_cache_state(const std::string &core_name);
};

#endif  // CACHE_H
