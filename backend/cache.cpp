#include "cache.h"
#include "bus.h"
#include <algorithm>

// Constructor
cache::cache(uint64_t index, bool moesi_protocol) {
    this->moesi_protocol = moesi_protocol;
    this->index = index;             // Asignar el índice recibido como parámetro
    data.fill(0);                    // Inicializa los datos con 0
    addresses.fill(0);               // Inicializa las direcciones con 0
    orden.fill(0);               // Inicializa las direcciones con 0
    moesi_state.fill("I");           // Inicializa el estado MOESI como "I" (Invalid)
    bus_access_enabled = true;       // Habilita el acceso al bus
    cache_misses = 0;                // Inicializa los fallos de cache en 0
    invalidations = 0;               // Inicializa las invalidaciones en 0

    for (int i= 0; i<8; i++){
        orden[i] = i;
    }
}


void cache::save_in_cache(std::string state, uint64_t addr, uint64_t dat){

    // Encuentra el iterador al menor elemento
    auto min_it = std::min_element(orden.begin(), orden.end());

    // Calcula el índice del menor elemento
    int min_index = std::distance(orden.begin(), min_it);

    // Asigna 8 al índice del menor elemento
    orden[min_index] = 8;

    // Resta 1 a todos los demás elementos
    for (size_t i = 0; i < orden.size(); ++i) {
        if (i != min_index) {
            --orden[i]; // Decrementa en 1
        }
    }

    // Imprime el índice del valor mínimo
    std::cout << "va en el bloque: " << min_index << std::endl;

    moesi_state[min_index] = state;
    addresses[min_index] = addr;
    data[min_index] = dat;

}


uint64_t cache::read(uint64_t addr,  bus& bus) {

    std::cout << "\n \n ****** Estado de la cache del " << index << " ****** \n";

    for (int i = 0; i < 8; ++i) {
        if (addresses[i] == addr){
            if (moesi_state[i] == "I"){
                cache_misses++;
                return bus.read_req_moesi(addr, index);
            } else {
                return data[i];
            }
        }
    }
    cache_misses++;
    return bus.read_req_moesi(addr, index);

}



void cache::write(int block, uint64_t addr, uint64_t data, bus& bus) {
    //bus.write_request(addr, data, index, block);
    if(this->moesi_protocol == false){
            bus.write_request_mesi(addr, data, index, block);
        }
    else{
            bus.write_request_moesi(addr, data, index, block);
    }
}

// Imprimir el estado de cada bloque en la cache
void cache::print_cache_state(const std::string &core_name) {
    std::cout << "\n \n ****** Estado de la cache del " << core_name << " ****** \n";
    for (int i = 0; i < 8; ++i) {
        std::cout << "\n------------------------------------------\n";
        std::cout << "Bloque " << i << "\n";
        std::cout << "Estado MOESI " << moesi_state[i] << "\n";
        std::cout << "Direccion RAM " << addresses[i] << "\n";
        std::cout << "Dato " << data[i] << "\n";
    }
}
