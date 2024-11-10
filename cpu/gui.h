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
    Gtk::Grid grid_mem, cache_0, cache_1, cache_2, cache_3,
                        cache_4, cpu_0,   cpu_1,   cpu_2,
                        cpu_3,   cpu_4;

    Gtk::Label example_text_label;


    Gtk::ScrolledWindow scrolled_window; // Contenedor con scroll
    Gtk::Box scroll_box; // Contenedor para los elementos dentro del scroll

    // Manejador de señal para el botón
    void on_button_clicked();

    // Manejador de señal para el botón
    void create_mem_scroll_bar();
};

#endif // GUI_H