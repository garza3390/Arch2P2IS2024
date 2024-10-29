#include "core.h"

// Función para cargar datos desde RAM a un registro
void core::load(int reg, uint64_t addr, RAM &memory, bus &system_bus) {
    int cache_block = addr % 8;  // Determinar el bloque de cache
    core_cache.load(cache_block, addr, memory, system_bus);
    registers[reg] = core_cache.data[cache_block];
}

// Función para almacenar datos de un registro en RAM
void core::store(int reg, uint64_t addr, RAM &memory, bus &system_bus) {
    int cache_block = addr % 8;  // Determinar el bloque de cache
    core_cache.store(cache_block, addr, memory, system_bus);
    memory.memory[addr] = registers[reg];
}

// Incrementar el valor en un registro 
void core::inc(int reg) {
    registers[reg]++;
}

// Decrementar el valor en un registro 
void core::dec(int reg) {
    registers[reg]--;
}

// Salto condicional si el valor en el registro no es cero 
void core::jnz(int reg, std::string label, instruction_memory &inst_mem) {
    if (registers[reg] != 0) {
        // Implementación vacía)
    }
}
