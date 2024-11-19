# Nombre del ejecutable
TARGET = moesi

# Archivos fuente y sus correspondientes archivos objeto
SRC = main.cpp bus.cpp cache.cpp core.cpp instructions_memory.cpp ram.cpp gui.cpp interface.cpp
OBJ = $(SRC:.cpp=.o)

# Compilador y opciones
CXX = g++
CXXFLAGS = -std=c++11 -pthread $(shell pkg-config --cflags gtkmm-3.0 --libs glib-2.0) # Flags de compilación para GTKmm y pthread

# Flags de enlace
LDFLAGS = $(shell pkg-config --libs gtkmm-3.0) -lpthread

# Regla por defecto
all: $(TARGET)

# Regla para compilar el ejecutable a partir de archivos objeto
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Regla para compilar cada archivo objeto
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Regla para limpiar los archivos generados
clean:
	rm -f *.o

# Regla para ejecutar el programa
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
