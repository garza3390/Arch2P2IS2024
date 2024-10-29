#ifndef CORE_H
#define CORE_H

//#include <cstdint>
#include <array>
#include <string>
#include "cache.h"
#include "ram.h"
#include "bus.h"
#include "instructions_memory.h"

struct core {
    cache core_cache;                         // Cada core tiene su propia memoria cache
    std::array<uint64_t, 4> registers;        // 4 registros de 64 bits: REG0, REG1, REG2, REG3

    // Función para cargar datos desde RAM a un registro
    void load(int reg, uint64_t addr, RAM &memory, bus &system_bus);

    // Función para almacenar datos de un registro en RAM
    void store(int reg, uint64_t addr, RAM &memory, bus &system_bus);

    // Incrementar el valor en un registro 
    void inc(int reg);

    // Decrementar el valor en un registro 
    void dec(int reg);

    // Salto condicional si el valor en el registro no es cero
    void jnz(int reg, std::string label, instruction_memory &inst_mem);
};

#endif  // CORE_H
