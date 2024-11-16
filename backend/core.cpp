#include "core.h"
#include "bus.h"
#include <iostream>
#include <thread>
#include <atomic>

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

uint64_t core::getCacheBlock(uint64_t ramAddress) {
    // Validación de rango
    if (ramAddress < 0 || ramAddress > 255) {
        std::cerr << "La dirección de RAM debe estar entre 0 y 255.\n";
        return -1; // Retorna -1 para indicar un error
    }
    
    // Correspondencia directa usando módulo 8
    return ramAddress % 8;
}

// Función que ejecuta las instrucciones
void core::start(bus& bus, std::atomic<bool>& stepper, std::atomic<bool>& step) {
    int current_line = 0;
    std::atomic<bool> clock(false);
    while (current_line < inst_mem.instructions.size()) {
        if(stepper.load()){
            clock = step.load();
        }
        else{
            clock = true;  // Semiciclo positivo
            std::this_thread::sleep_for(std::chrono::seconds(1));    
        }      
    
        const auto& inst = inst_mem.instructions[current_line];
        

        if (inst.mnemonic == "LOAD") {
            uint64_t cache_block = getCacheBlock(inst.regB);
            uint64_t data = load(cache_block, inst.regB, bus);
            registers[inst.regA] = data;
            current_line++;
        } else if (inst.mnemonic == "STORE") {
            uint64_t cache_block = getCacheBlock(inst.regB);
            store(cache_block, inst.regB, inst.regA, bus);
            current_line++;
        } else if (inst.mnemonic == "INC") {
            inc(inst.regA);
            current_line++;
        } else if (inst.mnemonic == "DEC") {
            dec(inst.regA);
            current_line++;
        } else if (inst.mnemonic == "JNZ") {
            current_line = jnz(inst.regA, inst.label, current_line);
        } else {
            std::cout << "\nEtiqueta: " << inst.mnemonic << std::endl;
            current_line++;
        }
        
        clock = false; // Semiciclo negativo
        std::this_thread::sleep_for(std::chrono::seconds(1));  
    }
}

