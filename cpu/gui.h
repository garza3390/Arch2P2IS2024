#ifndef GUI_H
#define GUI_H

#include <gtkmm.h>
#include <sstream>
#include <iostream>
#include <pango/pango.h> // Incluye Pango para manejar fuentes


class MiVentana : public Gtk::Window {
public:
    MiVentana(); // Constructor vacío
    virtual ~MiVentana();

    // Función para inicializar la ventana
    void init_window();
    void actualizar_mem_box(const long datos[256]);

private:


    Gtk::Button boton;
    Gtk::Grid grid_mem, cpu1_grid, cpu2_grid, cpu3_grid, cpu4_grid;

    Gtk::Label example_text_label;


    Gtk::ScrolledWindow scrolled_window; // Contenedor con scroll
    Gtk::Box scroll_box; // Contenedor para los elementos dentro del scroll

    // Manejador de señal para el botón
    void on_button_clicked(const std::string& label_text, 
                                  Gtk::ComboBoxText* instr_dropdown, 
                                  Gtk::ComboBoxText* reg_dropdown, 
                                  Gtk::Entry* addr_entry);


    //crea scrollbar
    void create_mem_scroll_bar();

    //crea cpus
    void create_cpu_grid(int row, int col, const std::string& label_text);
    void create_bus_grid();
    void create_info_grid();
    void create_mem_grid();


};
#endif // GUI_H