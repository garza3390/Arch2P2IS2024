#include "cache.h"

// Constructor para inicializar el estado MOESI
cache::cache() {
    for (int i = 0; i < 8; ++i) {
        moesi_state[i] = "I";  // Inicializamos todos los bloques en "Invalid"
    }
}

void cache::load(int block, uint64_t addr, RAM &memory, bus &system_bus) {
    if (moesi_state[block] == "I") {
        cache_misses++;
        moesi_state[block] = "S";  // Se asume estado compartido después de cargar
        data[block] = memory.memory[addr];
        system_bus.read_requests++;
        system_bus.data_transmitted += 64;  // Se transmiten 64 bits
    }
}

void cache::store(int block, uint64_t addr, RAM &memory, bus &system_bus) {
    if (moesi_state[block] == "I" || moesi_state[block] == "S") {
        moesi_state[block] = "M";  // Modificamos el bloque
    }
    data[block] = memory.memory[addr];  // Guardar en cache
    system_bus.write_requests++;
}

// Imprimir el estado de cada bloque en la cache
void cache::print_cache_state(const std::string &core_name) {
    std::cout << "Estado de la cache del " << core_name << ":\n";
    for (int i = 0; i < 8; ++i) {
        std::cout << "Bloque " << i << " - MOESI: " << moesi_state[i] << "\n";
    }
}
