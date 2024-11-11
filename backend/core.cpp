#include "core.h"
#include "bus.h"


// Función para cargar datos desde RAM a un registro
uint64_t core::load(int block, uint64_t addr, bus& bus) {
    uint64_t data = core_cache.read(block, addr, bus);
    return data;
}

// Función para almacenar datos de un registro en RAM
void core::store(int block, uint64_t addr, uint64_t data, bus& bus) {   
    core_cache.write(block, addr, data, bus);
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
        // Implementación vacía
    }
}
