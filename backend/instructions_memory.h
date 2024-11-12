#ifndef INSTRUCTION_MEMORY_H
#define INSTRUCTION_MEMORY_H

#include <vector>
#include <string>

struct Instruction {
    std::string operation;
    int block;
    uint64_t address;
    int reg;
    std::string label;
};

struct instruction_memory {
    std::vector<Instruction> instructions;

    // Constructor que recibe el índice de la cache y carga las instrucciones desde el archivo
    instruction_memory(uint64_t cache_index);

    // Función para leer una línea específica de las instrucciones
    std::string read_line(int line_num) const;

    // Función para encontrar el número de línea de una etiqueta
    int find_label_line(const std::string& label) const;
};

#endif // INSTRUCTION_MEMORY_H
