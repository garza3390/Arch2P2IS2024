#include "ram.h"
#include <iostream>

// Constructor
RAM::RAM() {
    memory.fill(9);  // Inicializa toda la memoria con ceros
}

// Lee desde una dirección de memoria y actualiza el bus
uint64_t RAM::read(uint64_t address) {
    if (address < memory.size()) {
        return memory[address];
    } else {
        std::cerr << "Error: Dirección fuera de los límites de la RAM." << std::endl;
        return 0;
    }
}

// Escribe en una dirección de memoria y actualiza el bus
void RAM::write(uint64_t address, uint64_t data) {
    if (address < memory.size()) {
        memory[address] = data;  // Almacena el dato en la dirección especificada
    } else {
        std::cerr << "Error: Dirección fuera de los límites de la RAM." << std::endl;
    }
}

// Imprime el estado de la memoria RAM
void RAM::print_ram_state() const {
    std::cout << "Estado de la RAM:" << std::endl;
    for (size_t i = 0; i < memory.size(); ++i) {
        std::cout << "Dirección " << i << ": " << memory[i] << std::endl;
    }
}
