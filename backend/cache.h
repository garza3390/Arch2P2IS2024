#ifndef CACHE_H
#define CACHE_H

//#include <cstdint>
#include <array>
#include <string>
#include <iostream>

struct bus;

struct cache {
    uint64_t index = 0;                       // indice para distinguir entre caches
    std::array<uint64_t, 8> data;             // 8 bloques de datos
    std::array<uint64_t, 8> addresses;        // Direcciones correspondientes
    std::array<std::string, 8> moesi_state;   // Estados MOESI por bloque
    bool bus_access_enabled = true;           // Acceso habilitado al bus
    int cache_misses = 0;
    int invalidations = 0;

    cache() : index(0) {}
    // Constructor
    cache(uint64_t index);

    uint64_t read(int block, uint64_t addr, bus& bus);

    void write(int block, uint64_t addr, uint64_t data, bus& bus);

    // Función para imprimir el estado de cada bloque en la cache
    void print_cache_state(const std::string &core_name);
};

#endif  // CACHE_H
