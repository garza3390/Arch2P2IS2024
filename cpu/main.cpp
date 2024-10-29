#include <thread>
#include "ram.h"
#include "cache.h"
#include "bus.h"
#include "core.h"


// Función para ejecutar un core
void execute_core(core &c, RAM &ram, bus &system_bus) {
    c.load(0, 10, ram, system_bus);  // Cargar memoria[10] en REG0
    c.inc(0);                        // Incrementar REG0
    c.store(0, 20, ram, system_bus); // Guardar REG0 en memoria[20]
}

int main() {
    RAM system_ram;
    bus system_bus;
    core core_0, core_1;

    // Inicializar memoria RAM
    for (int i = 0; i < 256; i++) {
        system_ram.memory[i] = i * 10;  // Valores iniciales en la memoria
    }

    // Crear hilos para ejecutar cores simultáneamente
    std::thread core_thread_0(execute_core, std::ref(core_0), std::ref(system_ram), std::ref(system_bus));
    std::thread core_thread_1(execute_core, std::ref(core_1), std::ref(system_ram), std::ref(system_bus));

    // Unir los hilos
    core_thread_0.join();
    core_thread_1.join();

    // Mostrar resultados básicos
    std::cout << "Número de peticiones de lectura: " << system_bus.read_requests << std::endl;
    std::cout << "Número de peticiones de escritura: " << system_bus.write_requests << std::endl;
    std::cout << "Cantidad de datos transmitidos: " << system_bus.data_transmitted << " bits" << std::endl;

    // Mostrar el estado MOESI de cada cache
    core_0.core_cache.print_cache_state("Core 0");
    core_1.core_cache.print_cache_state("Core 1");

    return 0;
}
