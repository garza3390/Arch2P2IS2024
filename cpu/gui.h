#ifndef GUI_H
#define GUI_H

#include <gtkmm.h>

class MiVentana : public Gtk::Window {
public:
    MiVentana(); // Constructor vacío
    virtual ~MiVentana();

    // Función para inicializar la ventana
    void init_window();

private:
    Gtk::Button boton;
    Gtk::Grid grid_mem, cache_0, cache_1, cache_2, cache_3,
                        cache_4, cpu_0,   cpu_1,   cpu_2,
                        cpu_3,   cpu_4;

    Gtk::Label example_text_label;

    // Manejador de señal para el botón
    void on_button_clicked();
};

#endif // GUI_H