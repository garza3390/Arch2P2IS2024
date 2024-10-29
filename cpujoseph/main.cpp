#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include "ram.h"
#include "cache.h"
#include "bus.h"
#include "core.h"

// Función para ejecutar un workload que simula false sharing
void workload_false_sharing(core &c, RAM &ram, bus &system_bus, int core_id) {
    // Simulación de false sharing
    for (int i = 0; i < 1000; ++i) {
        c.load(0, 0); // Cargar desde memoria[0]
        c.inc(0);     // Incrementar REG0
        c.store(0, 0); // Guardar en memoria[0]
        
        c.load(1, 1); // Cargar desde memoria[1]
        c.inc(1);     // Incrementar REG1
        c.store(1, 1); // Guardar en memoria[1]
    }
}

// Función para ejecutar un workload que evita false sharing
void workload_no_false_sharing(core &c, RAM &ram, bus &system_bus, int core_id) {
    // Simulación sin false sharing
    for (int i = 0; i < 1000; ++i) {
        c.load(0, core_id * 2); // Cargar desde memoria[0] o memoria[2]
        c.inc(0);               // Incrementar REG0
        c.store(0, core_id * 2); // Guardar en memoria[0] o memoria[2]
    }
}

int main() {
    RAM system_ram;
    bus system_bus;
    std::vector<core> cores(4); // 4 cores

    // Inicializar memoria RAM
    for (int i = 0; i < 256; i++) {
        system_ram.memory[i] = i * 10;  // Valores iniciales en la memoria
    }

    // Crear hilos para ejecutar workloads
    std::vector<std::thread> threads;
    
    // Simulación de false sharing
    std::cout << "Ejecutando workload que simula false sharing..." << std::endl;
    for (int i = 0; i < 4; ++i) {
        threads.emplace_back(workload_false_sharing, std::ref(cores[i]), std::ref(system_ram), std::ref(system_bus), i);
    }

    // Unir los hilos
    for (auto &t : threads) {
        t.join();
    }

    // Mostrar resultados básicos
    std::cout << "Resultados después de workload con false sharing:" << std::endl;
    std::cout << "Número de peticiones de lectura: " << system_bus.read_requests << std::endl;
    std::cout << "Número de peticiones de escritura: " << system_bus.write_requests << std::endl;
    std::cout << "Cantidad de datos transmitidos: " << system_bus.data_transmitted << " bits" << std::endl;

    // Mostrar el estado MOESI de cada cache
    for (int i = 0; i < 4; ++i) {
        cores[i].core_cache.print_cache_state("Core " + std::to_string(i));
    }

    // Limpiar hilos
    threads.clear();

    // Simulación sin false sharing
    std::cout << "\nEjecutando workload que evita false sharing..." << std::endl;
    for (int i = 0; i < 4; ++i) {
        threads.emplace_back(workload_no_false_sharing, std::ref(cores[i]), std::ref(system_ram), std::ref(system_bus), i);
    }

    // Unir los hilos
    for (auto &t : threads) {
        t.join();
    }
    std::cout << "Resultados después de workload sin false sharing:" << std::endl;
    std::cout << "Número de peticiones de lectura: " << system_bus.read_requests << std::endl;
    std::cout << "Número de peticiones de escritura: " << system_bus.write_requests << std::endl;
    std::cout << "Cantidad de datos transmitidos: " << system_bus.data_transmitted << " bits" << std::endl;

    // Mostrar el estado MOESI de cada cache
    for (int i = 0; i < 4; ++i) {
        cores[i].core_cache.print_cache_state("Core " + std::to_string(i));
    }

    return 0;
}