#include "cache.h"
#include "bus.h"

// Constructor
cache::cache(uint64_t index, bool moesi_protocol) {
    this->moesi_protocol = moesi_protocol;
    this->index = index;             // Asignar el índice recibido como parámetro
    data.fill(0);                    // Inicializa los datos con 0
    addresses.fill(0);               // Inicializa las direcciones con 0
    moesi_state.fill("I");           // Inicializa el estado MOESI como "I" (Invalid)
    bus_access_enabled = true;       // Habilita el acceso al bus
    cache_misses = 0;                // Inicializa los fallos de cache en 0
    invalidations = 0;               // Inicializa las invalidaciones en 0
}

uint64_t cache::read(int block, uint64_t addr,  bus& bus) {

    if (moesi_state[block] == "I") {  // Si el bloque está en estado inválido
        cache_misses++;
        uint64_t data_m = 0;
        //uint64_t data_m = bus.read_request(addr, index, block);

        if(this->moesi_protocol == false){
            data_m = bus.read_request_mesi(addr, index, block);
        }
        else{
            data_m = bus.read_request_moesi(addr, index, block);
        }

        addresses[block] = addr;
        data[block] = data_m;  // Carga el dato desde la memoria
        moesi_state[block] = bus.connected_caches[index]->moesi_state[block];

        //moesi_state[0] = bus.connected_caches[0].moesi_state[0];
        //std::cout << "Cache 0 bloque 0 - " <<  bus.connected_caches[0].moesi_state[0] << "\n";
        
        bus.read_requests++;
        bus.data_transmitted += 64;  // Se transmiten 64 bits

        return data_m;
    }
    else {
        return data[block];
    }
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
