# Nombre del ejecutable
TARGET = moesi

# Archivos fuente y sus correspondientes archivos objeto
SRC = main.cpp bus.cpp cache.cpp core.cpp instructions_memory.cpp ram.cpp
OBJ = $(SRC:.cpp=.o)

# Compilador y opciones
CXX = g++
CXXFLAGS = -std=c++11 -pthread

# Regla por defecto
all: $(TARGET)

# Regla para compilar el ejecutable a partir de archivos objeto
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Regla para compilar cada archivo objeto
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Regla para limpiar los archivos generados
clean:
	rm -f $(TARGET) $(OBJ)

# Regla para ejecutar el programa
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run
