#include "gui.h"

MiVentana::MiVentana() {

set_title("Proyecto II Arqui II"); 
set_default_size(1200, 800); 
set_resizable(false);

grid_mem.set_size_request(100, 100);
grid_mem.set_row_homogeneous(true); 
grid_mem.set_column_homogeneous(true); 




// Crear un nuevo grid interno
Gtk::Grid* cpu1_grid = Gtk::make_managed<Gtk::Grid>();
cpu1_grid->set_row_homogeneous(true);
cpu1_grid->set_column_homogeneous(true);

// Crear una etiqueta y añadirla al inner_grid
Gtk::Label* label = Gtk::make_managed<Gtk::Label>("CPU1");
cpu1_grid->attach(*label, 0, 0, 1, 1);

// Agregar el grid interno a la posición (j, i) en el grid principal
grid_mem.attach(*cpu1_grid, 0, 0, 1, 1);





// Crear un nuevo grid interno
Gtk::Grid* cpu2_grid = Gtk::make_managed<Gtk::Grid>();
cpu2_grid->set_row_homogeneous(true);
cpu2_grid->set_column_homogeneous(true);

// Crear una etiqueta y añadirla al inner_grid
Gtk::Label* label2 = Gtk::make_managed<Gtk::Label>("CPU2");
cpu2_grid->attach(*label2, 0, 0, 1, 1);

// Agregar el grid interno a la posición (j, i) en el grid principal
grid_mem.attach(*cpu2_grid, 0, 2, 1, 1);




// Crear un nuevo grid interno
Gtk::Grid* cpu3_grid = Gtk::make_managed<Gtk::Grid>();
cpu3_grid->set_row_homogeneous(true);
cpu3_grid->set_column_homogeneous(true);

// Crear una etiqueta y añadirla al inner_grid
Gtk::Label* label3 = Gtk::make_managed<Gtk::Label>("CPU3");
cpu3_grid->attach(*label3, 0, 0, 1, 1);

// Agregar el grid interno a la posición (j, i) en el grid principal
grid_mem.attach(*cpu3_grid, 2, 0, 1, 1);


// Crear un nuevo grid interno
Gtk::Grid* cpu4_grid = Gtk::make_managed<Gtk::Grid>();
cpu4_grid->set_row_homogeneous(true);
cpu4_grid->set_column_homogeneous(true);

// Crear una etiqueta y añadirla al inner_grid
Gtk::Label* label4 = Gtk::make_managed<Gtk::Label>("CPU4");
cpu4_grid->attach(*label4, 0, 0, 1, 1);

// Agregar el grid interno a la posición (j, i) en el grid principal
grid_mem.attach(*cpu4_grid, 2, 2, 1, 1);



// Crear un nuevo grid interno
Gtk::Grid* bus_grid = Gtk::make_managed<Gtk::Grid>();
bus_grid->set_row_homogeneous(true);
bus_grid->set_column_homogeneous(true);

// Crear una etiqueta y añadirla al inner_grid
Gtk::Label* label5 = Gtk::make_managed<Gtk::Label>("bus");
bus_grid->attach(*label5, 0, 0, 1, 1);

// Agregar el grid interno a la posición (j, i) en el grid principal
grid_mem.attach(*bus_grid, 0, 1, 3, 1);


// Crear un nuevo grid interno
Gtk::Grid* mem_grid = Gtk::make_managed<Gtk::Grid>();
mem_grid->set_row_homogeneous(true);
mem_grid->set_column_homogeneous(true);

create_mem_scroll_bar();
mem_grid->attach(scrolled_window, 0, 0, 1, 1);

// Agregar el grid interno a la posición (j, i) en el grid principal
grid_mem.attach(*mem_grid, 4, 0, 1, 3);


// Crear un nuevo grid interno
Gtk::Grid* info_grid = Gtk::make_managed<Gtk::Grid>();
info_grid->set_row_homogeneous(true);
info_grid->set_column_homogeneous(true);

// Crear una etiqueta y añadirla al inner_grid
Gtk::Label* label7 = Gtk::make_managed<Gtk::Label>("info");
info_grid->attach(*label7, 0, 0, 1, 1);

// Agregar el grid interno a la posición (j, i) en el grid principal
grid_mem.attach(*info_grid, 1, 0, 1, 1);


add(grid_mem);

show_all_children();

}

// Destructor
MiVentana::~MiVentana() {}

void MiVentana::init_window() {

}

void MiVentana::create_mem_scroll_bar() {

    // Configurar el contenedor con scroll
    scrolled_window.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_ALWAYS); // Scroll automático vertical y siempre horizontal
    scrolled_window.set_size_request(200, 600); // Tamaño del área con scroll (200px de ancho, 600px de alto)

    // Contenedor dentro del scroll (en este caso, un Gtk::Box)
    scroll_box.set_orientation(Gtk::ORIENTATION_VERTICAL); // Caja vertical para los elementos

    for (int i = 0; i < 256; ++i) {
        std::ostringstream oss;
        oss << "0x" << std::hex << std::uppercase << (i < 16 ? "0" : "") << i << "   :    00"; // Usa "26" como un valor de ejemplo
        std::string direccion = oss.str();

        Gtk::Label* label = new Gtk::Label(direccion);
        label->set_xalign(0.0f);

        // Configurar la fuente a "Consolas"
        Pango::FontDescription font_desc("Consolas 10"); // "12" es el tamaño de la fuente
        label->override_font(font_desc);

        scroll_box.pack_start(*label, Gtk::PACK_SHRINK);
    }

    // Agregar la caja al contenedor con scroll
    scrolled_window.add(scroll_box);
}

void MiVentana::actualizar_mem_box(const long datos[256]) {

    for (auto* child : scroll_box.get_children()) {
        scroll_box.remove(*child);
        delete child; // Eliminar los widgets para evitar fugas de memoria
    }

    // Crear y agregar los nuevos elementos con los datos del array
    for (int i = 0; i < 256; ++i) {
        std::ostringstream oss;
        oss << "0x" << std::hex << std::uppercase << (i < 16 ? "0" : "") << i << "   :    " << datos[i];
        std::string direccion = oss.str();

        Gtk::Label* label = new Gtk::Label(direccion);
        label->set_xalign(0.0f);

        // Configurar la fuente a "Consolas"
        Pango::FontDescription font_desc("Consolas 10"); // Tamaño de fuente "10"
        label->override_font(font_desc);

        scroll_box.pack_start(*label, Gtk::PACK_SHRINK);
    }

    // Mostrar los nuevos elementos
    show_all_children();
}


