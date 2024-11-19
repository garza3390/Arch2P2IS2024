#include "core.h"
#include "bus.h"
#include <iostream>

// Función para cargar datos desde RAM a un registro
void core::load(int reg, uint64_t addr, bus& bus) {
    //std::lock_guard<std::mutex> lock(bus_mutex);  // Proteger acceso al bus
    registers[reg] = core_cache.read(addr, bus);
}

// Función para almacenar datos de un registro en RAM
void core::store(int reg, uint64_t addr, bus& bus) {
    //std::lock_guard<std::mutex> lock(bus_mutex);  // Proteger acceso al bus
    //core_cache.write(block, addr, bus);
    core_cache.write(addr, registers[reg], bus);

    std::cout << "yendo a guardar en cache" << std::endl;
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
    std::cout << "Iniciando Core" << std::endl;
}

void core::run_instruccion(const std::string& selected_instr, const std::string& selected_reg, unsigned int addr, bus& bus){

    int reg = 0;

     if (selected_reg.rfind("REG", 0) == 0) {
        // Extrae la parte numérica como substring y convierte a entero
        reg =  std::stoi(selected_reg.substr(3));
    }


     if (selected_instr == "LOAD") {
            load(reg, addr, bus);
        } else if (selected_instr == "STORE") {
            store(reg, addr, bus);
        } else if (selected_instr == "INC") {
            inc(reg);
        } else if (selected_instr == "DEC") {
            dec(reg);
        } else if (selected_instr == "JNZ") {
            std::cout << "Etiqueta: " << selected_instr << std::endl;
        } else {
            std::cout << "Etiqueta: " << selected_instr << std::endl;
        }

}
