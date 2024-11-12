#include "instructions_memory.h"
#include <fstream>
#include <iostream>
#include <sstream>

instruction_memory::instruction_memory(uint64_t cache_index) {
    std::string filename = "instrucciones_" + std::to_string(cache_index) + ".txt";
    std::ifstream file(filename);

    if (!file) {
        std::cerr << "Error: No se pudo abrir el archivo " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        Instruction inst;
        iss >> inst.mnemonic;

        if (inst.mnemonic == "INC" || inst.mnemonic == "DEC") {
            // Leer solo el bloque
            iss >> inst.reg;
        } 
        else if (inst.mnemonic == "JNZ") {
            // Leer registro y etiqueta
            iss >> inst.reg >> inst.label;
        }
        else if (inst.mnemonic == "LOAD") {
            // Leer registro, bloque y dirección
            iss >> inst.reg >> inst.block >> inst.address;
        } else if (inst.mnemonic == "STORE") {
            // Leer registro, bloque, dato y dirección
            iss >> inst.reg >> inst.block >> inst.data >> inst.address;
            std::cout << "inst data " << inst.data;
        }

        // Agregar la instrucción al vector de instrucciones
        instructions.push_back(inst);
    }

    file.close();
}

Instruction instruction_memory::read_line(int line_num) {
    if (line_num >= 0 && line_num < instructions.size()) {
        return instructions[line_num];
    }
    return Instruction{}; // Devuelve una instancia vacía de Instruction
}

int instruction_memory::find_label_line(std::string label) {
    for (size_t i = 0; i < instructions.size(); ++i) {
        if (instructions[i].mnemonic == label) {
            return i;
        }
    }
    return -1;  // Devuelve -1 si no se encuentra la etiqueta
}
