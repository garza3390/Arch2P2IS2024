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


void cache::save_in_cache(std::string state, uint64_t addr, uint64_t dat, bus& bus){

    // Intentar encontrar el índice de `addr` en `addresses`
    auto addr_it = std::find(addresses.begin(), addresses.end(), addr);

    int target_index;

    if (addr_it != addresses.end()) {
        // Si se encuentra, calcular el índice
        target_index = std::distance(addresses.begin(), addr_it);
        std::cout << "Dirección encontrada en el índice: " << target_index << std::endl;
    } else {
        // Si no se encuentra, buscar el índice del menor elemento en `orden`
        auto min_it = std::min_element(orden.begin(), orden.end());
        target_index = std::distance(orden.begin(), min_it);
        std::cout << "Dirección no encontrada. Usando el índice del menor bloque: " << target_index << std::endl;
    }

    // Resta 1 a todos los demás elementos
    for (size_t i = 0; i < orden.size(); ++i) {
        if (i != target_index && orden[i] >= orden[target_index]) {
            --orden[i]; // Decrementa en 1
        }
    } 
    // Asignar 8 al índice del menor elemento
    orden[target_index] = 7;   


    // Imprimir el contenido de `orden`
    std::cout << "Contenido de 'orden' después de las modificaciones:" << std::endl;
    for (size_t i = 0; i < orden.size(); ++i) {
        std::cout << "orden[" << i << "] = " << orden[i] << std::endl;
    }  

    if (moesi_state[target_index] == "M"){
        bus.ram.write(addresses[target_index], data[target_index]);
    }
    
    moesi_state[target_index] = state;
    addresses[target_index] = addr;
    data[target_index] = dat;

}


uint64_t cache::read(uint64_t addr,  bus& bus) {

    std::cout << "\n \n ****** Estado de la cache del " << index << " ****** \n";

    for (int i = 0; i < 8; ++i) {
        if (addresses[i] == addr){
            if (moesi_state[i] == "E" || moesi_state[i] == "M" || moesi_state[i] == "S"){
                return data[i];
            } 
        }
    }
    cache_misses++;
    return bus.read_req_moesi(addr, index);
}



void cache::write(uint64_t addr, uint64_t data, bus& bus) {
    bool found = false; // Variable para verificar si la dirección fue encontrada
    
    for (int i = 0; i < 8; ++i) {
        if (addresses[i] == addr) {
            if (moesi_state[i]=="I"){
                read(addr, bus);
            }
            bus.write_req_moesi(i, data, index); // Realizar la escritura si se encuentra
            found = true; // Indicar que se encontró la dirección
            break; // Salir del bucle, ya que no es necesario seguir buscando
        }
    }

    if (!found) {
        read(addr, bus);
        for (int i = 0; i < 8; ++i) {
            if (addresses[i] == addr) {
                bus.write_req_moesi(i, data, index); // Realizar la escritura si se encuentra
            }
        }
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
