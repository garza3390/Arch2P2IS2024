#include "gui.h"

MiVentana::MiVentana() {

set_title("Proyecto II Arqui II"); 
set_default_size(1200, 800); 
set_resizable(false);

grid_mem.set_size_request(100, 100);
grid_mem.set_row_homogeneous(true); 
grid_mem.set_column_homogeneous(true); 

create_cpu_grid(0, 0, "CPU1");
create_cpu_grid(0, 2, "CPU2");
create_cpu_grid(2, 0, "CPU3");
create_cpu_grid(2, 2, "CPU4");

create_bus_grid();
create_info_grid();

create_mem_grid();


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




void MiVentana::create_cpu_grid(int row, int col, const std::string& label_text) {
    // Crear un nuevo grid interno
    Gtk::Grid* cpu_grid = Gtk::make_managed<Gtk::Grid>();
    cpu_grid->set_row_homogeneous(true);
    cpu_grid->set_column_homogeneous(true);

    // Crear una etiqueta y añadirla al grid interno
    Gtk::Label* label = Gtk::make_managed<Gtk::Label>(label_text);
    cpu_grid->attach(*label, 0, 0, 6, 1);

    // Crear 8 etiquetas y añadirlas al grid interno en 2 columnas y 4 filas
    for (int i = 0; i < 8; ++i) {
        Gtk::Label* label = Gtk::make_managed<Gtk::Label>("Etiqueta " + std::to_string(i + 1));
        
        // Calcular las filas y columnas
        int row_index = i / 2; // Esto divide las etiquetas en 4 filas
        int col_index = i % 2; // Esto asigna las etiquetas a las columnas 0 y 1

        cpu_grid->attach(*label, col_index*3, row_index+1, 3, 1); // Colocar las etiquetas en el grid
    }

    // Crear 3 menús desplegables (ComboBoxText)
    Gtk::ComboBoxText* instr_dropdown = Gtk::make_managed<Gtk::ComboBoxText>();
    Gtk::ComboBoxText* reg_dropdown = Gtk::make_managed<Gtk::ComboBoxText>();

    Gtk::Entry* addr_entry = Gtk::make_managed<Gtk::Entry>();
    addr_entry->set_max_length(4);  // Limitar la longitud de la entrada a 4 caracteres (para hex de 2 dígitos)
    addr_entry->set_text("0x00");  // Valor inicial

    // Establecer el ancho mínimo en caracteres (por ejemplo, 2 caracteres de ancho)
    addr_entry->set_width_chars(2);

    // Definir las listas de opciones
    std::vector<std::string> instr_options = {"LOAD", "STORE", "INC", "DEC", "JNZ"};
    std::vector<std::string> reg_options = {"REG0", "REG1", "REG2", "REG3"};

    // Añadir los elementos de las listas a los dropdowns
    for (const auto& option : instr_options) {
        instr_dropdown->append(option);
    }

    for (const auto& option : reg_options) {
        reg_dropdown->append(option);
    }


    // Colocar los dropdowns en el grid
    cpu_grid->attach(*instr_dropdown, 0, 6, 2, 1); // Colocar el primer dropdown en la posición (0, 2)
    cpu_grid->attach(*reg_dropdown, 2, 6, 2, 1); // Colocar el segundo dropdown en la posición (0, 3)
    cpu_grid->attach(*addr_entry, 4, 6, 2, 1); // Colocar el Entry en la posición adecuada




    // Crear un botón y añadirlo al grid interno
    Gtk::Button* button = Gtk::make_managed<Gtk::Button>("Run");
    button->signal_clicked().connect(sigc::bind(sigc::mem_fun(*this, &MiVentana::on_button_clicked), label_text, instr_dropdown, reg_dropdown, addr_entry));
    cpu_grid->attach(*button, 2, 7, 2, 1); // Colocamos el botón en la posición (0, 1)




    // Agregar el grid interno a la posición (col, row) en el grid principal
    grid_mem.attach(*cpu_grid, col, row, 1, 1);
}


void MiVentana::create_bus_grid() {
    // Crear un nuevo grid interno
    Gtk::Grid* cpu_grid = Gtk::make_managed<Gtk::Grid>();
    cpu_grid->set_row_homogeneous(true);
    cpu_grid->set_column_homogeneous(true);

    // Crear una etiqueta y añadirla al grid interno
    Gtk::Label* label = Gtk::make_managed<Gtk::Label>("bus");
    cpu_grid->attach(*label, 0, 0, 1, 1);

    // Agregar el grid interno a la posición (col, row) en el grid principal
    grid_mem.attach(*cpu_grid, 0, 1, 3, 1);
}


void MiVentana::create_info_grid() {
    // Crear un nuevo grid interno
    Gtk::Grid* cpu_grid = Gtk::make_managed<Gtk::Grid>();
    cpu_grid->set_row_homogeneous(true);
    cpu_grid->set_column_homogeneous(true);

    // Crear una etiqueta y añadirla al grid interno
    Gtk::Label* label = Gtk::make_managed<Gtk::Label>("info");
    cpu_grid->attach(*label, 0, 0, 1, 1);

    // Agregar el grid interno a la posición (col, row) en el grid principal
    grid_mem.attach(*cpu_grid, 1, 0, 1, 1);
}


void MiVentana::create_mem_grid() {
    // Crear un nuevo grid interno
    Gtk::Grid* mem_grid = Gtk::make_managed<Gtk::Grid>();
    mem_grid->set_row_homogeneous(true);
    mem_grid->set_column_homogeneous(true);

    create_mem_scroll_bar();
    mem_grid->attach(scrolled_window, 0, 0, 1, 1);

    // Agregar el grid interno a la posición (j, i) en el grid principal
    grid_mem.attach(*mem_grid, 4, 0, 1, 3);
}

// Manejador de la señal clicked del botón
void MiVentana::on_button_clicked(const std::string& label_text, 
                                  Gtk::ComboBoxText* instr_dropdown, 
                                  Gtk::ComboBoxText* reg_dropdown, 
                                  Gtk::Entry* addr_entry) {
    // Obtener las opciones seleccionadas de los dropdowns
    std::string selected_instr = instr_dropdown->get_active_text();
    std::string selected_reg = reg_dropdown->get_active_text();
    std::string addr_text = addr_entry->get_text(); // Obtener el texto del Entry

    // Validar la entrada hexadecimal en el Entry
    if (addr_text.size() == 4 && addr_text[0] == '0' && addr_text[1] == 'x') {
        try {
            // Convertir el texto a número hexadecimal
            int value;
            std::stringstream ss;
            ss << std::hex << addr_text.substr(2); // Convertir el texto después de "0x"
            ss >> value;

            // Verificar si está dentro del rango permitido
            if (value >= 0 && value <= 0xFF) {
                std::cout << "Se presionó el botón de la CPU con ID: " << label_text << std::endl;
                std::cout << "Instrucción seleccionada: " << selected_instr << std::endl;
                std::cout << "Registro seleccionado: " << selected_reg << std::endl;
                std::cout << "Dirección ingresada: 0x" << std::hex << value << std::endl;
            } else {
                std::cout << "Dirección fuera de rango (debe ser entre 0x0 y 0xFF)." << std::endl;
            }
        } catch (...) {
            std::cout << "Error al convertir la dirección hexadecimal." << std::endl;
        }
    } else {
        std::cout << "Formato incorrecto de la dirección. Debe ser 0x seguido de 2 dígitos hexadecimales." << std::endl;
    }

    // Limpiar las opciones seleccionadas de los dropdowns
    instr_dropdown->set_active(-1); // Deseleccionar cualquier opción en instr_dropdown
    reg_dropdown->set_active(-1); // Deseleccionar cualquier opción en reg_dropdown
    addr_entry->set_text("0x00"); // Limpiar el texto del Entry
}