#include "bus.h"
#include <iostream>

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
