#include "bus.h"
#include "core.h"
#include "ram.h"

int main() {
    RAM ram;
    core core0(0); 
    core core1(1); 
    core core2(2);
    core core3(3);
    bus bus(core0, core1, core2, core3, ram);
    uint64_t loaded_data = 0;

    uint64_t address = 10; 
    uint64_t block = 0;
    uint64_t data = 369;
    
    std::cout << "\n------------------------------------------\n";

    // Primera lectura en Core 0 (debe ir a estado Exclusive E)
    loaded_data = core0.load(block, address, bus);
    
    // Imprimir estados para verificar el cumplimiento del protocolo
    core0.core_cache.print_cache_state("Core 0");
    
    std::cout << "\n------------------------------------------\n";
    
    // Lectura en Core 3 (core 0 block 0 estado O, core 3 block 0 estado S)
    loaded_data = core3.load(block, address, bus);
    
    
    // Imprimir estados para verificar el cumplimiento del protocolo
    core0.core_cache.print_cache_state("Core 0");
    core3.core_cache.print_cache_state("Core 3");


    
    std::cout << "\n------------------------------------------\n";


    /*
    std::cout << "\n------------------------------------------\n";
    
    //bus.print_bus_state();
    //memory.print_ram_state();
     */

    return 0;
}
