#include "gui.h"
#include <iostream>

MiVentana::MiVentana() {

set_title("Proyecto II Arqui II"); 
set_default_size(1200, 800); 
set_resizable(false);

grid_mem.set_size_request(100, 100);
grid_mem.set_row_homogeneous(true); 
grid_mem.set_column_homogeneous(true); 

// Crear 4 etiquetas con el mismo texto
for (int i = 0; i < 4; ++i) {
    Gtk::Label* example_text_label = new Gtk::Label("Texto ejemplo");
    grid_mem.attach(*example_text_label, 0, i, 1, 1); // Añadir a cada fila, única columna
}

add(grid_mem);

show_all_children();

}

// Destructor
MiVentana::~MiVentana() {}

void MiVentana::init_window() {

}

