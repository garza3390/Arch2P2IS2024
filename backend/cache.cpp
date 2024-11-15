#include "cache.h"
#include "bus.h"
#include <nlohmann/json.hpp>
#include <fstream>

// Constructor
cache::cache(uint64_t index, bool moesi_protocol) {
    this->moesi_protocol = moesi_protocol;
    this->index = index;             // Asignar el índice recibido como parámetro
    data.fill(0);                    // Inicializa los datos con 0
    addresses.fill(0);               // Inicializa las direcciones con 0
    moesi_state.fill("I");           // Inicializa el estado MOESI como "I" (Invalid)
    bus_access_enabled = true;       // Habilita el acceso al bus
    cache_hits = 0;
    cache_misses = 0;                // Inicializa los fallos de cache en 0
    invalidations = 0;               // Inicializa las invalidaciones en 0
}

uint64_t cache::read(int block, uint64_t addr, bus& bus) {
    std::cout << "\nEntrando en cache::read" << std::endl;
    std::cout << "Block: " << block << ", Address: " << addr << std::endl;

    if (moesi_state[block] == "I") {  // Si el bloque está en estado inválido
        std::cout << "Estado del bloque es 'I', aumentando cache_misses" << std::endl;
        cache_misses++;

        uint64_t data_m = 0;
        if (this->moesi_protocol == false) {
            data_m = bus.read_request_mesi(addr, index, block);
        } else {
            data_m = bus.read_request_moesi(addr, index, block);
        }

        std::cout << "Datos leídos de la memoria o bus: " << data_m << std::endl;
        addresses[block] = addr;
        data[block] = data_m;  // Carga el dato desde la memoria

        // Verificar que el estado de MOESI sea válido
        std::cout << "Actualizando estado MOESI" << std::endl;
        moesi_state[block] = bus.connected_caches[index]->moesi_state[block];

        bus.read_requests++;
        bus.data_transmitted += 64;  // Se transmiten 64 bits

        std::cout << "Saliendo de cache::read con data: " << data_m << std::endl;
        return data_m;
    } else {
        std::cout << "Bloque no está en estado 'I', devolviendo data[block]" << std::endl;
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


void cache::save_metrics_to_json(const std::string& core_name) const {
    nlohmann::json metrics;

    // Almacena las métricas en la estructura JSON
    metrics["core ID"] = core_name;
    metrics["cache_hits"] = cache_hits;
    metrics["cache_misses"] = cache_misses;
    metrics["invalidations"] = invalidations;

    // Guarda las métricas en un archivo JSON
    std::ofstream file(core_name + "_cache_metrics.json");
    if (file.is_open()) {
        file << metrics.dump(4);  // Formato con indentación de 4 espacios
        file.close();
        std::cout << "Métricas guardadas en " << core_name << "_cache_metrics.json" << std::endl;
    } else {
        std::cerr << "Error al abrir el archivo para guardar las métricas" << std::endl;
    }
}
