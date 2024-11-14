#ifndef BUS_H
#define BUS_H

#include <array>
#include <cstdint>
#include <string>
#include <iostream>
#include "cache.h"
#include "ram.h"
#include "core.h"
#include <vector>
#include <chrono>

// Bus del sistema
struct bus {

    mutable std::mutex metrics_mutex;

    struct MetricsSnapshot {
            float time_elapsed;  // Tiempo en segundos desde el inicio
            int read_requests;
            int write_requests;
            int invalidations;
            int data_transmitted;
            int read_responses;
            int write_responses;
        };

    std::vector<MetricsSnapshot> metrics_log;

    std::array<uint64_t, 256> address_port;  // Puerto de direcciones (conectado a RAM y cache de cada core)
    std::array<uint64_t, 256> data_port;     // Puerto de datos (conectado a RAM y cache de cada core)
    std::vector<cache*> connected_caches;   // Puerto compartido para estados MOESI entre caches de los cores
    
    RAM ram;

    // Contadores para eventos del bus
    int read_requests = 0;
    int write_requests = 0;
    int invalidations = 0;
    int read_responses = 0;
    int write_responses = 0;
    uint64_t data_transmitted = 0;

    // Constructor
    bus(core& core0, core& core1, core& core2, core& core3, RAM& ram);

    void update_cache();

    // Función para manejar una solicitud de lectura en el bus
    uint64_t read_request_moesi(uint64_t address, uint64_t cache_index, uint64_t cache_block);

    // Función para manejar una solicitud de escritura en el bus
    void write_request_moesi(uint64_t address, uint64_t data, uint64_t cache_index, uint64_t cache_block);

    // Función para manejar una solicitud de lectura en el bus
    uint64_t read_request_mesi(uint64_t address, uint64_t cache_index, uint64_t cache_block);

    // Función para manejar una solicitud de escritura en el bus
    void write_request_mesi(uint64_t address, uint64_t data, uint64_t cache_index, uint64_t cache_block);

    // Función para imprimir el estado del bus
    void print_bus_state() const;

    // Escribir las metricas en un archivo json
    void save_metrics_to_json(const std::vector<core*>& cores) const;

    void update_metrics_snapshot(float time_elapsed);

    // Variables de tiempo
    std::chrono::time_point<std::chrono::steady_clock> start_time;
};

#endif // BUS_H
