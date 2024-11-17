#include "bus.h"
#include <iostream>
#include <nlohmann/json.hpp>
#include <fstream>

// instalar el paquete nlohmann con el comando
// comando: sudo apt-get install nlohmann-json3-dev


// Constructor del bus
bus::bus(core& core0, core& core1, core& core2, core& core3, RAM& ram, MiVentana* ventana)
    : address_port{0}, data_port{0}, ram(ram), ventana(ventana){
    
    connected_caches.resize(4);
    connected_caches[0] = &core0.core_cache;
    connected_caches[1] = &core1.core_cache;
    connected_caches[2] = &core2.core_cache;
    connected_caches[3] = &core3.core_cache;

    cores.resize(4);
    cores[0] = &core0;
    cores[1] = &core1;
    cores[2] = &core2;
    cores[3] = &core3;

    read_requests = 0;
    write_requests = 0;
    invalidations = 0;
    data_transmitted = 0;
    read_responses = 0;
    write_responses = 0;

}

void bus::update_cache() {
    
    // Imprimir estados para verificar el cumplimiento del protocolo
    connected_caches[0]->print_cache_state("Core 0");
    //core1.core_cache.print_cache_state("Core 1");
    //core2.core_cache.print_cache_state("Core 2");
    //core3.core_cache.print_cache_state("Core 3");
    std::cout << "actualizando" << std::endl;

    for (int j = 0; j < 4; ++j) {
        for (int i = 0; i < 8; ++i) {
        ventana->actualizar_cache(j+1, i+1, 
                                connected_caches[j]->moesi_state[i], 
                                std::to_string(connected_caches[j]->addresses[i]), 
                                std::to_string(connected_caches[j]->data[i]));
        }
    }
    for (int j = 0; j < 4; ++j) {
        for (int i = 0; i < 4; ++i) {
        ventana->actualizar_reg(j+1, i, cores[j]->registers[i]);
        }
    }
}

// Función para manejar una solicitud de lectura en el bus
uint64_t bus::read_request_moesi(uint64_t address, uint64_t cache_index, uint64_t cache_block) {

    
    
    read_requests++;
    uint64_t data = 0;
    
    // Revisar si la dirección está en la caché local
    if (connected_caches[cache_index]->moesi_state[cache_block] != "I") {
        // Si no está en estado "I", se encuentra en la caché
        data = connected_caches[cache_index]->data[cache_block];
        data_port[address] = data;
        address_port[address] = address;

        //update_moesi_state(address, data, cache_index, cache_block);
        std::cout << "Bus: Lectura completada desde la caché local en la dirección " << address << " con el dato " << data << std::endl;
        update_cache();
    } else {
        
        // Si la caché local está en estado "I", buscar en las otras cachés
        bool found = false;
        for (int i = 0; i < connected_caches.size(); ++i) {
            if (i != cache_index) {
                // Buscar en las otras cachés por la misma dirección
                for (int block = 0; block < 8; ++block) {
                    if (connected_caches[i]->addresses[block] == address) {
                        // Si encontramos el bloque, revisamos el estado MOESI
                        std::string& other_state = connected_caches[i]->moesi_state[block];

                        if (other_state == "M" || other_state == "O" || other_state == "E" || other_state == "S") {
                            // Si el estado es "M", "O", "E" o "S", tomamos el dato
                            data = connected_caches[i]->data[block];

                            // Actualizamos las cachés con la nueva dirección y dato
                            connected_caches[cache_index]->data[cache_block] = data;
                            connected_caches[cache_index]->addresses[cache_block] = address;

                            if (other_state == "E"){
                                connected_caches[cache_index]->moesi_state[cache_block] = "S";
                                connected_caches[i]->moesi_state[block] = "O";
                            }
                            else if (other_state == "O"){
                                connected_caches[cache_index]->moesi_state[cache_block] = "S";
                                connected_caches[i]->moesi_state[block] = "O";
                            }
                            else if (other_state == "M" || other_state == "S"){
                                connected_caches[cache_index]->moesi_state[cache_block] = "S";
                                connected_caches[i]->moesi_state[block] = "S";

                            }
                            

                            // Actualizar los puertos del bus
                            data_port[address] = data;
                            address_port[address] = address;
                            

                            //std::cout << connected_caches[i].moesi_state[block] << "\n";
                            //std::cout << connected_caches[cache_index].moesi_state[cache_block] << "\n";

                            std::cout << "Bus: Lectura completada desde caché " << i << " en la dirección " << address << " con el dato " << data << std::endl;

                            update_cache();
                            found = true;
                            break;
                        }
                    }
                }
            }
            if (found) break;
        }

        if (!found) {
            // Si no se encontró el dato en ninguna caché, leer de la RAM
            data = ram.read(address);
            data_port[address] = data;
            address_port[address] = address;
            connected_caches[cache_index]->data[cache_block] = data;
            connected_caches[cache_index]->addresses[cache_block] = address;
            connected_caches[cache_index]->moesi_state[cache_block] = "E";
            std::cout << "Bus: Lectura completada desde RAM en la dirección " << address << " con el dato " << data << std::endl;
            update_cache();
        }



    }
    
    return data;
}

// Función para manejar una solicitud de escritura en el bus
void bus::write_request_moesi(uint64_t address, uint64_t data, uint64_t cache_index, uint64_t cache_block) {

    
    if (connected_caches[cache_index]->moesi_state[cache_block] == "S") {
        connected_caches[cache_index]->moesi_state[cache_block] = "M";  
    }
    else if (connected_caches[cache_index]->moesi_state[cache_block] == "I"){
        connected_caches[cache_index]->moesi_state[cache_block] = "O"; 
    }
    connected_caches[cache_index]->data[cache_block] = data;  // Guardar en cache

    write_requests++;
    data_port[address] = data;
    address_port[address] = address;
    data_transmitted += sizeof(data);

    for (int i = 0; i < connected_caches.size(); ++i) {
            if (i != cache_index) {
                // Buscar en las otras cachés por la misma dirección
                for (int block = 0; block < 8; ++block) {
                    if (connected_caches[i]->addresses[block] == address) {
                        connected_caches[i]->moesi_state[block] = "I";
                        connected_caches[i]->invalidations++;
                        
                        //connected_caches[i]->moesi_state[block] = "S";
                        //connected_caches[i]->data[block] = data;
                    }
                }
            }
        }
    std::cout << "Bus: Escritura completada en la cache " << cache_index << " en el bloque " << cache_block << " con el dato " << data << std::endl;
    update_cache();
}


// Función para manejar una solicitud de lectura en el bus
uint64_t bus::read_request_mesi(uint64_t address, uint64_t cache_index, uint64_t cache_block) {
    


    read_requests++;
    uint64_t data = 0;
    
    // Revisar si la dirección está en la caché local
    if (connected_caches[cache_index]->moesi_state[cache_block] != "I") {
        // Si no está en estado "I", se encuentra en la caché
        data = connected_caches[cache_index]->data[cache_block];
        data_port[address] = data;
        address_port[address] = address;

        //update_moesi_state(address, data, cache_index, cache_block);
        std::cout << "Bus: Lectura completada desde la caché local en la dirección " << address << " con el dato " << data << std::endl;
        update_cache();
    } else {
        
        // Si la caché local está en estado "I", buscar en las otras cachés
        bool found = false;
        for (int i = 0; i < connected_caches.size(); ++i) {
            if (i != cache_index) {
                // Buscar en las otras cachés por la misma dirección
                for (int block = 0; block < 8; ++block) {
                    if (connected_caches[i]->addresses[block] == address) {
                        // Si encontramos el bloque, revisamos el estado MOESI
                        std::string& other_state = connected_caches[i]->moesi_state[block];

                        if (other_state == "M" || other_state == "O" || other_state == "E" || other_state == "S") {
                            // Si el estado es "M", "O", "E" o "S", tomamos el dato
                            data = connected_caches[i]->data[block];

                            // Actualizamos las cachés con la nueva dirección y dato
                            connected_caches[cache_index]->data[cache_block] = data;
                            connected_caches[cache_index]->addresses[cache_block] = address;

                            if (other_state == "E"){
                                connected_caches[cache_index]->moesi_state[cache_block] = "S";
                                connected_caches[i]->moesi_state[block] = "S";
                            }
                            else if (other_state == "O"){
                                connected_caches[cache_index]->moesi_state[cache_block] = "S";
                                connected_caches[i]->moesi_state[block] = "S";
                            }
                            else if (other_state == "M" || other_state == "S"){
                                connected_caches[cache_index]->moesi_state[cache_block] = "S";
                                connected_caches[i]->moesi_state[block] = "S";

                            }
                            

                            // Actualizar los puertos del bus
                            data_port[address] = data;
                            address_port[address] = address;
                            

                            //std::cout << connected_caches[i].moesi_state[block] << "\n";
                            //std::cout << connected_caches[cache_index].moesi_state[cache_block] << "\n";

                            std::cout << "Bus: Lectura completada desde caché " << i << " en la dirección " << address << " con el dato " << data << std::endl;
                            update_cache();
                            found = true;
                            break;
                        }
                    }
                }
            }
            if (found) break;
        }

        if (!found) {
            // Si no se encontró el dato en ninguna caché, leer de la RAM
            data = ram.read(address);
            data_port[address] = data;
            address_port[address] = address;
            connected_caches[cache_index]->data[cache_block] = data;
            connected_caches[cache_index]->addresses[cache_block] = address;
            connected_caches[cache_index]->moesi_state[cache_block] = "E";
            std::cout << "Bus: Lectura completada desde RAM en la dirección " << address << " con el dato " << data << std::endl;
            update_cache();
        }
    }
    
    return data;
}

// Función para manejar una solicitud de escritura en el bus
void bus::write_request_mesi(uint64_t address, uint64_t data, uint64_t cache_index, uint64_t cache_block) {
    if (connected_caches[cache_index]->moesi_state[cache_block] == "S") {
        connected_caches[cache_index]->moesi_state[cache_block] = "M";  
    }
    else if (connected_caches[cache_index]->moesi_state[cache_block] == "I"){
        connected_caches[cache_index]->moesi_state[cache_block] = "E"; 
    }
    connected_caches[cache_index]->data[cache_block] = data;  // Guardar en cache

    write_requests++;
    data_port[address] = data;
    address_port[address] = address;
    data_transmitted += sizeof(data);

    for (int i = 0; i < connected_caches.size(); ++i) {
            if (i != cache_index) {
                // Buscar en las otras cachés por la misma dirección
                for (int block = 0; block < 8; ++block) {
                    if (connected_caches[i]->addresses[block] == address) {
                        connected_caches[i]->moesi_state[block] = "I";
                        connected_caches[i]->invalidations++;
                        //connected_caches[i]->moesi_state[block] = "S";
                        //connected_caches[i]->data[block] = data;
                    }
                }
            }
        }
    std::cout << "Bus: Escritura completada en la cache " << cache_index << " en el bloque " << cache_block << " con el dato " << data << std::endl;
    update_cache();
}




// Función para imprimir el estado del bus
void bus::print_bus_state() const {
    std::cout << "Estado del Bus:\n";
    std::cout << "Read Requests: " << read_requests << "\n";
    std::cout << "Write Requests: " << write_requests << "\n";
    std::cout << "Invalidations: " << invalidations << "\n";
    std::cout << "Data Transmitted: " << data_transmitted << " bytes\n";
}


void bus::save_metrics_to_json(const std::vector<core*>& cores) const {
    
    std::lock_guard<std::mutex> lock(metrics_mutex);  // Protege la sección crítica
    nlohmann::json metrics_json;

    // Almacena las métricas del bus en la estructura JSON
    metrics_json["summary"]["read_requests"] = read_requests;
    metrics_json["summary"]["write_requests"] = write_requests;
    metrics_json["summary"]["invalidations"] = invalidations;
    metrics_json["summary"]["data_transmitted"] = data_transmitted;
    metrics_json["summary"]["read_responses"] = read_responses;
    metrics_json["summary"]["write_responses"] = write_responses;

    // Variables para almacenar los totales de cache hits, misses e invalidaciones de los cores
    uint64_t total_cache_hits = 0;
    uint64_t total_cache_misses = 0;
    uint64_t total_invalidations = 0;

    // Suma de las estadísticas de los cores
    for (const auto& core_ptr : cores) {
        total_cache_hits += core_ptr->core_cache.cache_hits;
        total_cache_misses += core_ptr->core_cache.cache_misses;
        total_invalidations += core_ptr->core_cache.invalidations;
    }

    // Agrega los totales de los cores al archivo JSON
    metrics_json["cores_total"]["total_cache_hits"] = total_cache_hits;
    metrics_json["cores_total"]["total_cache_misses"] = total_cache_misses;
    metrics_json["cores_total"]["total_invalidations"] = total_invalidations;

    // Registro de métricas por intervalo de tiempo
    nlohmann::json log_json = nlohmann::json::array();
    for (const auto& snapshot : metrics_log) {
        nlohmann::json snapshot_json;
        snapshot_json["time_elapsed"] = snapshot.time_elapsed;
        snapshot_json["read_requests"] = snapshot.read_requests;
        snapshot_json["write_requests"] = snapshot.write_requests;
        snapshot_json["invalidations"] = snapshot.invalidations;
        snapshot_json["data_transmitted"] = snapshot.data_transmitted;
        snapshot_json["read_responses"] = snapshot.read_responses;
        snapshot_json["write_responses"] = snapshot.write_responses;
        log_json.push_back(snapshot_json);
    }
    metrics_json["metrics_over_time"] = log_json;

    // Guarda las métricas en un archivo JSON
    std::ofstream file("bus_metrics.json");
    if (file.is_open()) {
        file << metrics_json.dump(4);  // Formato con indentación de 4 espacios
        file.close();
        std::cout << "Métricas guardadas en bus_metrics.json" << std::endl;
    } else {
        std::cerr << "Error al abrir el archivo para guardar las métricas" << std::endl;
    }
}


void bus::update_metrics_snapshot(float time_elapsed) {
    std::lock_guard<std::mutex> lock(metrics_mutex);  // Protege la sección crítica

    MetricsSnapshot snapshot;
    snapshot.time_elapsed = time_elapsed;
    snapshot.read_requests = read_requests;
    snapshot.write_requests = write_requests;
    snapshot.invalidations = invalidations;
    snapshot.data_transmitted = data_transmitted;
    snapshot.read_responses = read_responses;
    snapshot.write_responses = write_responses;

    metrics_log.push_back(snapshot);
}
