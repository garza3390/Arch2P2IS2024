# Arch2P2IS2024: MultiProcessor with Threads

## Descripción

Este proyecto implementa un sistema multiprocesador utilizando hilos en C++. El sistema simula la interacción de cuatro núcleos con una memoria caché, un bus de comunicación y una memoria RAM. El protocolo de coherencia de caché MOESI (Modified, Owned, Exclusive, Shared, Invalid) se utiliza para garantizar la consistencia de los datos entre las cachés y la memoria principal.

## Funcionalidades Principales

- **Caché por núcleo**: Cada núcleo tiene su propia memoria caché que interactúa con el bus para cargar y almacenar datos.
- **Protocolos MESI y MOESI**: Implementación para manejar coherencia de caché entre núcleos.
- **Ejecución en paralelo**: Los núcleos se ejecutan simultáneamente utilizando hilos (`std::thread`) para simular un entorno multiprocesador.
- **Captura de métricas**: El sistema recolecta estadísticas de las operaciones en el bus, como solicitudes de lectura/escritura y datos transmitidos, y las guarda en formato JSON.

## Estructura del Código

- **`main.cpp`**:
  - Crea e inicializa los núcleos, el bus y la memoria RAM.
  - Ejecuta instrucciones almacenadas en la memoria de instrucciones de cada núcleo.
  - Administra la sincronización entre los hilos y el manejo de métricas.

- **`core.cpp`**:
  - Define la lógica para cargar, almacenar y modificar datos.
  - Implementa instrucciones como `LOAD`, `STORE`, `INC`, `DEC` y `JNZ`.
  - Realiza la gestión de bloques de caché y acceso al bus.

- **`bus.cpp`**:
  - Implementa la lógica del protocolo MOESI.
  - Administra solicitudes de lectura/escritura desde los núcleos hacia la memoria principal o entre cachés.
  - Mantiene métricas de rendimiento como solicitudes atendidas y datos transmitidos.

## Ejecución del Programa

1. **Compilación**:
   Asegúrate de tener instalado un compilador de C++ compatible con C++17 o superior. Compila el programa con el siguiente comando:
   ```bash
   g++ -std=c++17 -o arch2p2is main.cpp core.cpp bus.cpp ram.cpp -lpthread -lnlohmann_json
