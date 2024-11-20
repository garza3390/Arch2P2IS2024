#include "gui.h"
#include <iostream>
#include <iomanip>

MiVentana::MiVentana(bus* bus): mibus(bus) {


set_title("Proyecto II Arqui II"); 
set_default_size(1000, 800); 
set_resizable(false);

grid_mem.set_size_request(100, 100);
grid_mem.set_row_homogeneous(true); 
grid_mem.set_column_homogeneous(true); 


grid_mem.set_margin_top(50);   // Margen superior de 50px
grid_mem.set_margin_bottom(50); // Margen inferior de 50px
grid_mem.set_margin_start(50);  // Margen izquierdo de 50px

create_cpu_grid(0, 0, "CPU1");
create_cpu_grid(0, 2, "CPU2");
create_cpu_grid(3, 0, "CPU3");
create_cpu_grid(3, 2, "CPU4");

create_bus_grid();
create_mem_grid();


add(grid_mem);

show_all_children();
actualizar();

}

// Destructor
MiVentana::~MiVentana() {
    
}

void MiVentana::init_window() {

}

void MiVentana::create_mem_scroll_bar() {

    // Configurar el contenedor con scroll
    scrolled_window.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_ALWAYS); // Scroll automático vertical y siempre horizontal
    scrolled_window.set_size_request(200, 600); // Tamaño del área con scroll (200px de ancho, 600px de alto)

    // Contenedor dentro del scroll (en este caso, un Gtk::Box)
    scroll_box.set_orientation(Gtk::ORIENTATION_VERTICAL); // Caja vertical para los elementos

    // Establecer un padding a la izquierda de 20 píxeles
    scroll_box.set_margin_left(100);

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

void MiVentana::actualizar_mem_box(const std::array<unsigned long, 256>& datos) {
    // Eliminar los widgets actuales para evitar fugas de memoria
    for (auto* child : scroll_box.get_children()) {
        scroll_box.remove(*child);
        delete child;
    }

    // Crear y agregar nuevos elementos basados en el array de datos
    for (size_t i = 0; i < datos.size(); ++i) {
        std::ostringstream oss;
        oss << "0x" << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << i 
            << "   :    " << datos[i];
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
    cpu_grid->attach(*label, 0, 0, 1, 1);

    Gtk::Label* cache_mises_label = Gtk::make_managed<Gtk::Label>("Cache Mises: 0");
    cpu_grid->attach(*cache_mises_label, 2, 0, 2, 1); // Colocar cada etiqueta en una columna del sub-grid
        
    // Etiqueta en blanco al final para conservar el espacio
    Gtk::Label* invalidaciones_label = Gtk::make_managed<Gtk::Label>("Invalidaciones: 0");
    cpu_grid->attach(*invalidaciones_label, 4, 0, 2, 1); // Colocar la etiqueta en blanco en la columna 5


    etiquetas_misses_inv.push_back(cache_mises_label);
    etiquetas_misses_inv.push_back(invalidaciones_label);


    // Crear 8 etiquetas y añadirlas al grid interno en 2 columnas y 4 filas
    for (int i = 0; i < 8; ++i) {
        // Crear un nuevo sub-grid para las 3 etiquetas
        Gtk::Grid* sub_grid = Gtk::make_managed<Gtk::Grid>();
        sub_grid->set_row_homogeneous(true);  // Opcional: hace que las filas sean homogéneas en tamaño
        sub_grid->set_column_homogeneous(true);
        

        // Etiqueta con el número de `i` al inicio
        Gtk::Label* index_label = Gtk::make_managed<Gtk::Label>(std::to_string(i + 1) + ".");
        sub_grid->attach(*index_label, 0, 0, 1, 1); // Colocar la etiqueta al inicio en la columna 0

        Gtk::Label* mesi_label = Gtk::make_managed<Gtk::Label>("M");
        sub_grid->attach(*mesi_label, 1, 0, 1, 1); // Colocar cada etiqueta en una columna del sub-grid

        Gtk::Label* addr_label = Gtk::make_managed<Gtk::Label>("0x00");
        sub_grid->attach(*addr_label, 2, 0, 2, 1); // Colocar cada etiqueta en una columna del sub-grid

        Gtk::Label* data_label = Gtk::make_managed<Gtk::Label>("0");
        sub_grid->attach(*data_label, 4, 0, 1, 1); // Colocar cada etiqueta en una columna del sub-grid
        
        // Crear un objeto EtiquetasCache y almacenarlo en la lista
        EtiquetasCache cache = { mesi_label, addr_label, data_label };
        etiquetas_cache.push_back(cache);   
        // Calcular las filas y columnas
        int row_index = i / 2; // Esto divide las etiquetas en 4 filas
        int col_index = i % 2; // Esto asigna las etiquetas a las columnas 0 y 1

        cpu_grid->attach(*sub_grid, col_index*3, row_index+1, 3, 1); // Colocar las etiquetas en el grid
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



    // Crear un nuevo sub-grid para las 3 etiquetas
        Gtk::Grid* reg_grid = Gtk::make_managed<Gtk::Grid>();
        reg_grid->set_row_homogeneous(true);  // Opcional: hace que las filas sean homogéneas en tamaño
        reg_grid->set_column_homogeneous(true);
        

        // Etiqueta con el número de `i` al inicio
        Gtk::Label* reg0_label = Gtk::make_managed<Gtk::Label>("REG0: 0");
        reg_grid->attach(*reg0_label, 0, 0, 1, 1); // Colocar la etiqueta al inicio en la columna 0

        Gtk::Label* reg1_label = Gtk::make_managed<Gtk::Label>("REG1: 0");
        reg_grid->attach(*reg1_label, 1, 0, 1, 1); // Colocar cada etiqueta en una columna del sub-grid

        Gtk::Label* reg2_label = Gtk::make_managed<Gtk::Label>("REG2: 0");
        reg_grid->attach(*reg2_label, 2, 0, 1, 1); // Colocar cada etiqueta en una columna del sub-grid

        Gtk::Label* reg3_label = Gtk::make_managed<Gtk::Label>("REG3: 0");
        reg_grid->attach(*reg3_label, 3, 0, 1, 1); // Colocar cada etiqueta en una columna del sub-grid


        
        etiquetas_reg.push_back(reg0_label);  
        etiquetas_reg.push_back(reg1_label);  
        etiquetas_reg.push_back(reg2_label);  
        etiquetas_reg.push_back(reg3_label);  


        cpu_grid->attach(*reg_grid, 0, 8, 6, 1);
        
    


    // Agregar el grid interno a la posición (col, row) en el grid principal
    grid_mem.attach(*cpu_grid, col, row, 1, 2);
}


void MiVentana::create_bus_grid() {
    // Crear un nuevo grid interno
    Gtk::Grid* grid = Gtk::make_managed<Gtk::Grid>();
    grid->set_row_homogeneous(true);
    grid->set_column_homogeneous(true);

    // Crear una etiqueta y añadirla al grid interno
    Gtk::Label* label = Gtk::make_managed<Gtk::Label>("BUS");
    grid->attach(*label, 0, 0, 1, 1);


    Gtk::Label* inv_label = Gtk::make_managed<Gtk::Label>("Invalidaciones: 0");
    grid->attach(*inv_label, 1, 0, 1, 1); // Colocar cada etiqueta en una columna del sub-grid
        
    Gtk::Label* read_label = Gtk::make_managed<Gtk::Label>("Read Request: 0");
    grid->attach(*read_label, 2, 0, 1, 1); // Colocar cada etiqueta en una columna del sub-grid

    Gtk::Label* write_label = Gtk::make_managed<Gtk::Label>("Write Request: 0");
    grid->attach(*write_label, 3, 0, 1, 1); // Colocar cada etiqueta en una columna del sub-grid

    Gtk::Label* dat_0_label = Gtk::make_managed<Gtk::Label>("Datos a Core 1: 0");
    grid->attach(*dat_0_label, 0, 1, 1, 1); // Colocar cada etiqueta en una columna del sub-grid

    Gtk::Label* dat_1_label = Gtk::make_managed<Gtk::Label>("Datos a Core 2: 0");
    grid->attach(*dat_1_label, 1, 1, 1, 1); // Colocar cada etiqueta en una columna del sub-grid

    Gtk::Label* dat_2_label = Gtk::make_managed<Gtk::Label>("Datos a Core 3: 0");
    grid->attach(*dat_2_label, 2, 1, 1, 1); // Colocar cada etiqueta en una columna del sub-grid

    Gtk::Label* dat_3_label = Gtk::make_managed<Gtk::Label>("Datos a Core 4: 0");
    grid->attach(*dat_3_label, 3, 1, 1, 1); // Colocar cada etiqueta en una columna del sub-grid
    



    etiquetas_bus.push_back(inv_label);
    etiquetas_bus.push_back(read_label);
    etiquetas_bus.push_back(write_label);
    etiquetas_bus.push_back(dat_0_label);
    etiquetas_bus.push_back(dat_1_label);
    etiquetas_bus.push_back(dat_2_label);
    etiquetas_bus.push_back(dat_3_label);



    // Agregar el grid interno a la posición (col, row) en el grid principal
    grid_mem.attach(*grid, 0, 2, 3, 1);
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
    grid_mem.attach(*mem_grid, 4, 0, 1, 5);
}


// Función externa para modificar el texto de una etiqueta específica
void MiVentana::actualizar_cache(int cpu, int cache, const std::string& mesi_text, const std::string& addr_text, const std::string& data_text) {

    // Comprobar si cpu es menor a 4 y cache es menor a 9
    if (cpu < 5 && cache < 9) {
        // Verificar que el índice de cache está dentro del rango válido
        if (cache < etiquetas_cache.size()) {
            // Obtener las etiquetas correspondientes para esta cache
            EtiquetasCache& etiqueta = etiquetas_cache[(cpu-1)*8 + (cache-1)];

            // Convertir addr_text a entero, luego a hexadecimal
            int addr_value = std::stoi(addr_text);  // Convertir addr_text a entero
            std::stringstream addr_hex;
            addr_hex << std::hex << addr_value;     // Convertir a hexadecimal

            // Actualizar las etiquetas de mesi, addr y data
            etiqueta.mesi_label->set_text(mesi_text);
            etiqueta.addr_label->set_text("0x" + addr_hex.str());
            etiqueta.data_label->set_text(data_text);
        }
    } else {
        // Si cpu >= 4 o cache >= 9, no se hace nada o se puede manejar de otra forma
        std::cout << "Error: CPU debe ser menor que 4 y Cache debe ser menor que 9." << std::endl;
    }
}

// Función externa para modificar el texto de una etiqueta específica
void MiVentana::actualizar_reg(int cpu, int reg, int data) {

    //std::cout << "editando reg" << reg << "de core " << cpu << "con dato " << data << std::endl;
    // Comprobar si cpu es menor a 4 y cache es menor a 9
    if (cpu < 5 && reg < 4) {
        // Verificar que el índice de cache está dentro del rango válido
        if (reg < etiquetas_reg.size()) {
            // Obtener el puntero a la etiqueta correspondiente
            Gtk::Label* etiqueta = etiquetas_reg[(cpu-1)*4 + reg];
            
            // Actualizar el texto de la etiqueta
            etiqueta->set_text("REG" + std::to_string(reg) + ": " + std::to_string(data));
        }
    } else {
        // Si cpu >= 4 o cache >= 9, no se hace nada o se puede manejar de otra forma
        std::cout << "Error: CPU debe ser menor que 4 y Cache debe ser menor que 9." << std::endl;
    }
}



// Función externa para modificar el texto de una etiqueta específica
void MiVentana::actualizar_misses_inv(int cpu, int tipo, int data) {
    std::cout << "actualizando el " << cpu << "en tipo " << tipo <<std::endl; 

    //std::cout << "editando reg" << reg << "de core " << cpu << "con dato " << data << std::endl;
    // Comprobar si cpu es menor a 4 y cache es menor a 9
    if (cpu < 5 && tipo < 3) {
        Gtk::Label* etiqueta = etiquetas_misses_inv[(cpu-1)*2 + tipo];
        std::cout << (cpu-1)*4 + tipo << " colectada de " << etiquetas_misses_inv.size()<< std::endl; 
        // Verificar que el índice de cache está dentro del rango válido
        if (tipo == 0) {
            // Actualizar el texto de la etiqueta
            etiqueta->set_text("Cache Mises: " + std::to_string(data));
        } else if (tipo == 1){
            etiqueta->set_text("Invalidaciones: " + std::to_string(data));
        }
    } else {
        // Si cpu >= 4 o cache >= 9, no se hace nada o se puede manejar de otra forma
        std::cout << "Error: CPU debe ser menor que 4 y Cache debe ser menor que 9." << std::endl;
    }
}

// Función externa para modificar el texto de una etiqueta específica
void MiVentana::actualizar_bus(int inv, int read, int write, int datos_0, int datos_1, int datos_2, int datos_3) {

    std::cout << "Actualizando bus" << std::endl;


    etiquetas_bus[0]->set_text("Invalidaciones: " + std::to_string(inv));
    etiquetas_bus[1]->set_text("Read Request: " + std::to_string(read));
    etiquetas_bus[2]->set_text("Write Request: " + std::to_string(write));
    etiquetas_bus[3]->set_text("Datos a Core 1: " + std::to_string(datos_0));
    etiquetas_bus[4]->set_text("Datos a Core 2: " + std::to_string(datos_1));
    etiquetas_bus[5]->set_text("Datos a Core 3: " + std::to_string(datos_2));
    etiquetas_bus[6]->set_text("Datos a Core 4: " + std::to_string(datos_3));
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


                if (label_text == "CPU1") {
                    mibus->cores[0]->run_instruccion(selected_instr, selected_reg, value, *mibus);
                } else if (label_text == "CPU2") {
                    mibus->cores[1]->run_instruccion(selected_instr, selected_reg, value, *mibus);
                } else if (label_text == "CPU3") {
                    mibus->cores[2]->run_instruccion(selected_instr, selected_reg, value, *mibus);
                } else if (label_text == "CPU4") {
                    mibus->cores[3]->run_instruccion(selected_instr, selected_reg, value, *mibus);
                } else {
                    std::cout << "Core no encontrado" << std::endl;
                }

                

                actualizar();

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

void MiVentana::actualizar(){
    for (int j = 0; j < 4; ++j) {
        for (int i = 0; i < 8; ++i) {
            actualizar_cache(j+1, i+1, 
                            mibus->cores[j]->core_cache.moesi_state[i], 
                            std::to_string(mibus->cores[j]->core_cache.addresses[i]), 
                            std::to_string(mibus->cores[j]->core_cache.data[i]));
        }
    }
    
    std::cout << "acutalizando " << std::endl;

    int inv_total = 0;
    for (int j = 0; j < 4; ++j) {
        actualizar_misses_inv(j+1, 0, mibus->cores[j]->core_cache.cache_misses);
        actualizar_misses_inv(j+1, 1, mibus->cores[j]->core_cache.invalidations);
        inv_total = inv_total + mibus->cores[j]->core_cache.invalidations;
    }

    for (int j = 0; j < 4; ++j) {
        for (int i = 0; i < 4; ++i) {
            actualizar_reg(j+1, i, mibus->cores[j]->registers[i]);
        }
    }


    for (int j = 0; j < 4; ++j) {
        for (int i = 0; i < 4; ++i) {
            actualizar_reg(j+1, i, mibus->cores[j]->registers[i]);
        }
    }


    actualizar_bus(inv_total, mibus->read_requests, mibus->write_requests, mibus->data_trans[0], mibus->data_trans[1], mibus->data_trans[2], mibus->data_trans[3]);
    actualizar_mem_box(mibus->ram.memory);

    
}


void MiVentana::gestion_LOAD(int cpu, const std::string& addr_text, const std::string& data_text){
    
}

void MiVentana::gestion_STORE(int cpu, const std::string& addr_text, const std::string& data_text){
    
}

void MiVentana::gestion_DEC(int cpu, int reg, int data){

}

void MiVentana::gestion_INC(int cpu, int reg, int data){ 
    
}


// Constructor para las columnas de PEs
TablaDatos::ColumnasPE::ColumnasPE() {
    add(pe);
    add(cache_misses);
    add(invalidaciones);
    add(datos_transmitidos);
    add(cache_hits);
    add(instrucciones);
}

// Constructor para las columnas del bus
TablaDatos::ColumnasBus::ColumnasBus() {
    add(invalidaciones);
    add(read_request);
    add(write_request);
    add(instrucciones_procesadas);
}

// Constructor principal
TablaDatos::TablaDatos(bus* bus): mibus(bus) {
    set_title("Tablas de datos: PEs y Bus");
    set_default_size(800, 600);

    // Configurar modelo y vista para los PEs

    list_store_pe = Gtk::ListStore::create(columnas_pe);
    tree_view_pe.set_model(list_store_pe);
    tree_view_pe.append_column("PE", columnas_pe.pe);
    tree_view_pe.append_column("Cache Misses", columnas_pe.cache_misses);
    tree_view_pe.append_column("Cache Hits", columnas_pe.cache_hits);
    tree_view_pe.append_column("Instrucciones", columnas_pe.instrucciones);
    tree_view_pe.append_column("Invalidaciones", columnas_pe.invalidaciones);
    tree_view_pe.append_column("Datos Transmitidos", columnas_pe.datos_transmitidos);
    cargar_datos_pe();

    std::cout << "actualiza pe " << std::endl;
    scrolled_window_pe.add(tree_view_pe);
    scrolled_window_pe.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

    // Configurar modelo y vista para el bus
    list_store_bus = Gtk::ListStore::create(columnas_bus);
    tree_view_bus.set_model(list_store_bus);
    tree_view_bus.append_column("Invalidaciones", columnas_bus.invalidaciones);
    tree_view_bus.append_column("Read Requests", columnas_bus.read_request);
    tree_view_bus.append_column("Write Requests", columnas_bus.write_request);
    tree_view_bus.append_column("Instrucciones Procesadas", columnas_bus.instrucciones_procesadas);
    cargar_datos_bus();
    std::cout << "actualiza bus " << std::endl;

    scrolled_window_bus.add(tree_view_bus);
    scrolled_window_bus.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

    // Agregar las tablas al layout principal
    main_box.set_orientation(Gtk::ORIENTATION_VERTICAL);
    main_box.pack_start(scrolled_window_pe, Gtk::PACK_EXPAND_WIDGET);
    main_box.pack_start(scrolled_window_bus, Gtk::PACK_EXPAND_WIDGET);
    add(main_box);

    show_all_children();
}

// Cargar datos de los PEs
void TablaDatos::cargar_datos_pe() {


    std::vector<std::string> pes = {"PE1", "PE2", "PE3", "PE4"};

    for (int i = 0; i < 4; ++i) {
        auto row = *(list_store_pe->append());
        row[columnas_pe.pe] = pes[i];
        row[columnas_pe.cache_misses] = mibus->cores[i]->core_cache.cache_misses;
        row[columnas_pe.cache_hits] = mibus->cores[i]->core_cache.cache_hits;
        row[columnas_pe.instrucciones] = mibus->cores[i]->instrucciones;
        row[columnas_pe.invalidaciones] = mibus->cores[i]->core_cache.invalidations;
        row[columnas_pe.datos_transmitidos] = mibus->data_trans[i];
    }

}

// Cargar datos del bus
void TablaDatos::cargar_datos_bus() {
    int inv_total = 0;
    int instr_total = 0;
    for (int j = 0; j < 4; ++j) {
        inv_total = inv_total + mibus->cores[j]->core_cache.invalidations;
        instr_total = instr_total + mibus->cores[j]->instrucciones;
    }

    auto row = *(list_store_bus->append());
    row[columnas_bus.invalidaciones] = inv_total;
    row[columnas_bus.read_request] = mibus->read_requests;
    row[columnas_bus.write_request] = mibus->write_requests;
    row[columnas_bus.instrucciones_procesadas] = instr_total;
}
