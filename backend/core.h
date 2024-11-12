#ifndef CORE_H
#define CORE_H

#include <array>
#include <string>
#include "cache.h"
#include <mutex>
#include "instructions_memory.h"

struct bus;

struct core {
    cache core_cache;                         // Cada core tiene su propia memoria cache
    std::array<uint64_t, 4> registers = {0};  // 4 registros de 64 bits, inicializados a 0
    instruction_memory inst_mem;
    std::mutex bus_mutex;

    // Constructor que inicializa el índice de la cache
    core(int index, bool moesi_protocol) : core_cache(index, moesi_protocol), inst_mem(index) {}

    // Función para cargar datos desde cache o RAM en caso de un miss
    uint64_t load(int block, uint64_t addr, bus& bus);

    // Función para almacenar datos en cache
    void store(int block, uint64_t addr, uint64_t data, bus& bus);

    // Incrementar el valor en un registro 
    void inc(int reg);

    // Decrementar el valor en un registro 
    void dec(int reg);

    // Salto condicional si el valor en el registro no es cero
    int jnz(int reg, const std::string& label, int current_line);

    // Función que ejecuta las instrucciones
    void start(bus& bus);
};

#endif  // CORE_H
