#ifndef RAM_H
#define RAM_H

#include <array>
#include <cstdint>

struct RAM {
    std::array<uint64_t, 256> memory;  // Memoria del sistema con 256 posiciones de 64 bits

    // Constructor
    RAM();

    // Función para leer desde una dirección de memoria usando el bus
    uint64_t read(uint64_t address);

    // Función para escribir en una dirección de memoria usando el bus
    void write(uint64_t address, uint64_t data);

    // Función para imprimir el estado de la memoria RAM
    void print_ram_state() const;
};

#endif // RAM_H
