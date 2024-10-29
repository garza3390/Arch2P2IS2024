#ifndef CACHE_H
#define CACHE_H

#include <array>
#include <string>
#include "ram.h"
#include "bus.h"

struct cache {
    std::array<uint64_t, 256> data;  // Datos en la cache
    std::array<std::string, 256> addresses;  // Direcciones en la cache
    std::array<std::string, 256> moesi_state;  // Estado MOESI de cada bloque

    // Funciones para manejar la cache
    void load(int block, uint64_t addr, RAM &memory, bus &system_bus);
    void store(int block, uint64_t addr, RAM &memory, bus &system_bus);
    void print_cache_state(const std::string &core_name);

    // Funciones para implementar MESI y MOESI
    void mesi_load(int block, uint64_t addr, RAM &memory, bus &system_bus);
    void moesi_load(int block, uint64_t addr, RAM &memory, bus &system_bus);
    void mesi_store(int block, uint64_t addr, RAM &memory, bus &system_bus);
    void moesi_store(int block, uint64_t addr, RAM &memory, bus &system_bus);
};

#endif