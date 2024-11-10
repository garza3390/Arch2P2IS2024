#include "bus.h"
#include "core.h"
#include "ram.h"

int main() {
    RAM ram;
    core core0(0); 
    core core1(1); 
    core core2(2);
    core core3(3);
    bus system_bus(core0, core1, core2, core3, ram);

    uint64_t address = 66; 
    uint64_t block = 2;
    uint64_t data = 369;
    
    std::cout << "\n------------------------------------------\n";

    // Primera lectura en Core A (debe ir a estado Exclusive E)
    uint64_t loaded_data = core2.load(block, address, system_bus);
    std::cout << "Dato leido de cache (si hay miss va a memoria): " << loaded_data << "\n";
    // Imprimir estados para verificar el cumplimiento del protocolo
    core2.core_cache.print_cache_state("Core 0");

    std::cout << "\n------------------------------------------\n";
    
    /*
    // Core B intenta leer el mismo dato (debe cambiar a estado Shared S en ambos cores)
    coreB.load(block, address, system_bus, memory);

    // Imprimir estados para verificar el cumplimiento del protocolo
    coreA.core_cache.print_cache_state("Core A");
    coreB.core_cache.print_cache_state("Core B");

    std::cout << "\n------------------------------------------\n";
    
    // Core A intenta escribir en el mismo dato (debe pasar a Modified M en Core A y Invalid I en Core B)
    coreA.store(block, address, data, system_bus);

    // Imprimir estados para verificar el cumplimiento del protocolo
    coreA.core_cache.print_cache_state("Core A");
    coreB.core_cache.print_cache_state("Core B");

    std::cout << "\n------------------------------------------\n";
    
    // Core B intenta leer el dato después de invalidación (debe pasar a Owner O en Core A y Shared S en Core B)
    coreB.load(block, address, system_bus, memory);

    // Imprimir estados para verificar el cumplimiento del protocolo
    coreA.core_cache.print_cache_state("Core A");
    coreB.core_cache.print_cache_state("Core B");

    std::cout << "\n------------------------------------------\n";
    
    //system_bus.print_bus_state();
    //memory.print_ram_state();
     */

    return 0;
}
