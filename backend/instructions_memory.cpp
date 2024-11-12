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
        iss >> inst.operation >> inst.block >> inst.address >> inst.reg >> inst.label;
        instructions.push_back(inst);
    }
    file.close();
}

std::string instruction_memory::read_line(int line_num) const {
    if (line_num >= 0 && line_num < instructions.size()) {
        return instructions[line_num].operation;
    }
    return "";
}

int instruction_memory::find_label_line(const std::string& label) const {
    for (size_t i = 0; i < instructions.size(); ++i) {
        if (instructions[i].operation == label) {
            return i;
        }
    }
    return -1;  // Devuelve -1 si no se encuentra la etiqueta
}
