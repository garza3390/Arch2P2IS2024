#ifndef BUS_H
#define BUS_H

#include <array>
#include <cstdint>
#include <string>

// Bus del sistema
struct bus {
    std::array<uint64_t, 256> address_port;  // Puerto de direcciones (conectado a RAM y cache de cada core)
    std::array<uint64_t, 256> data_port;     // Puerto de datos (conectado a RAM y cache de cada core)
    std::string moesi_port;  // Puerto compartido para estados MOESI entre caches de los cores

    // Contadores para eventos del bus
    int read_requests = 0;
    int write_requests = 0;
    int invalidations = 0;
    uint64_t data_transmitted = 0;
};

#endif // BUS_H
