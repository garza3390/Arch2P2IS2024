#include "core.h"
#include "bus.h"
#include <iostream>

// Función para cargar datos desde RAM a un registro
uint64_t core::load(int block, uint64_t addr, bus& bus) {
    std::lock_guard<std::mutex> lock(bus_mutex);  // Proteger acceso al bus
    return core_cache.read(block, addr, bus);
}

// Función para almacenar datos de un registro en RAM
void core::store(int block, uint64_t addr, uint64_t data, bus& bus) {
    std::lock_guard<std::mutex> lock(bus_mutex);  // Proteger acceso al bus
    core_cache.write(block, addr, data, bus);
}

// Incrementar el valor en un registro 
void core::inc(int reg) {
    registers[reg]++;
}

// Decrementar el valor en un registro 
void core::dec(int reg) {
    registers[reg]--;
}

// Salto condicional si el valor en el registro no es cero 
int core::jnz(int reg, const std::string& label, int current_line) {
    if (registers[reg] != 0) {
        int line_num = inst_mem.find_label_line(label);
        if (line_num != -1) {
            return line_num;  // Salta a la línea de la etiqueta
        } else {
            std::cerr << "Etiqueta no encontrada: " << label << std::endl;
        }
    }
    return current_line + 1;  // Avanza a la siguiente línea si no hay salto
}

// Función que ejecuta las instrucciones
void core::start(bus& bus) {
    int current_line = 0;
    while (current_line < inst_mem.instructions.size()) {
        const auto& inst = inst_mem.instructions[current_line];

        if (inst.operation == "LOAD") {
            uint64_t data = load(inst.block, inst.address, bus);
            registers[inst.reg] = data;
            current_line++;
        } else if (inst.operation == "STORE") {
            store(inst.block, inst.address, registers[inst.reg], bus);
            current_line++;
        } else if (inst.operation == "INC") {
            inc(inst.reg);
            current_line++;
        } else if (inst.operation == "DEC") {
            dec(inst.reg);
            current_line++;
        } else if (inst.operation == "JNZ") {
            current_line = jnz(inst.reg, inst.label, current_line);
        } else {
            std::cerr << "Instrucción desconocida: " << inst.operation << std::endl;
            current_line++;
        }
    }
}
