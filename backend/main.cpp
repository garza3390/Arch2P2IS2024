#include "bus.h"
#include "core.h"
#include "ram.h"
#include <thread>

int main() {
    bool moesi_protocol = false;

    RAM ram;
    core core0(0, moesi_protocol); 
    core core1(1, moesi_protocol); 
    core core2(2, moesi_protocol);
    core core3(3, moesi_protocol);
    bus bus(core0, core1, core2, core3, ram);
    bus.start_time = std::chrono::steady_clock::now();

    std::vector<core*> cores = {&core0, &core1, &core2, &core3};
    
    // Crear hilos para ejecutar el método start en cada core
    std::vector<std::thread> threads;
    threads.emplace_back(&core::start, &core0, std::ref(bus));
    threads.emplace_back(&core::start, &core1, std::ref(bus));
    threads.emplace_back(&core::start, &core2, std::ref(bus));
    threads.emplace_back(&core::start, &core3, std::ref(bus));


    bool running = true;
    std::thread metrics_thread([&]() {
        while (running) {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));  // Captura de métricas cada 100 ms para mayor granularidad
            auto current_time = std::chrono::steady_clock::now();
            float time_elapsed = std::chrono::duration<float>(current_time - bus.start_time).count();
            bus.update_metrics_snapshot(time_elapsed);
        }
    });



    // Esperar a que todos los hilos terminen
    for (auto& thread : threads) {
        thread.join();
    }

    running = false;
    metrics_thread.join();

    // Imprimir estados para verificar el cumplimiento del protocolo
    core0.core_cache.print_cache_state("Core 0");
    core1.core_cache.print_cache_state("Core 1");
    core2.core_cache.print_cache_state("Core 2");
    core3.core_cache.print_cache_state("Core 3");

    core0.core_cache.save_metrics_to_json("Core 0");
    core1.core_cache.save_metrics_to_json("Core 1");
    core2.core_cache.save_metrics_to_json("Core 2");
    core3.core_cache.save_metrics_to_json("Core 3");
    bus.save_metrics_to_json(cores);

    return 0;
}
