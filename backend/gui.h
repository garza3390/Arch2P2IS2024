#ifndef GUI_H
#define GUI_H

#include <gtkmm.h>
#include <sstream>
#include <iostream>
#include <pango/pango.h> // Incluye Pango para manejar fuentes
#include "bus.h"


class MiVentana : public Gtk::Window {
public:
    MiVentana(bus* bus); // Constructor vacío
    virtual ~MiVentana();

    // Función para inicializar la ventana
    void init_window();
    void actualizar_mem_box(const std::array<unsigned long, 256>& datos);
    void actualizar_cache(int cpu, int cache, const std::string& mesi_text, const std::string& addr_text, const std::string& data_text);
    void actualizar_reg(int cpu, int reg, int data);
    void actualizar_misses_inv(int cpu, int tipo, int data);
    void actualizar_bus(int inv, int read, int write, int datos_0, int datos_1, int datos_2, int datos_3);
    // Estructura que almacena las etiquetas de cada grupo
    struct EtiquetasCache {
        Gtk::Label* mesi_label;
        Gtk::Label* addr_label;
        Gtk::Label* data_label;
    };


private:

    bus* mibus; // Agrega un puntero a MiVentana

    std::vector<EtiquetasCache> etiquetas_cache;
    std::vector<Gtk::Label*> etiquetas_reg;
    std::vector<Gtk::Label*> etiquetas_misses_inv;
    std::vector<Gtk::Label*> etiquetas_bus;


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
    void actualizar();


    void gestion_LOAD(int cpu, const std::string& addr_text, const std::string& data_text);
    void gestion_STORE(int cpu, const std::string& addr_text, const std::string& data_text);
    void gestion_DEC(int cpu, int reg, int data);
    void gestion_INC(int cpu, int reg, int data);

};


class TablaDatos : public Gtk::Window {
public:
    TablaDatos(bus* bus);

private:
    bus* mibus; // Agrega un puntero a MiVentana
    // Widgets principales
    Gtk::TreeView tree_view_pe;
    Gtk::TreeView tree_view_bus;
    Gtk::ScrolledWindow scrolled_window_pe;
    Gtk::ScrolledWindow scrolled_window_bus;
    Gtk::Box main_box;

    // Modelos de datos
    Glib::RefPtr<Gtk::ListStore> list_store_pe;
    Glib::RefPtr<Gtk::ListStore> list_store_bus;

    // Columnas para los PEs
    class ColumnasPE : public Gtk::TreeModel::ColumnRecord {
    public:
        ColumnasPE();
        Gtk::TreeModelColumn<std::string> pe;
        Gtk::TreeModelColumn<int> cache_misses;
        Gtk::TreeModelColumn<int> cache_hits;
        Gtk::TreeModelColumn<int> instrucciones;
        Gtk::TreeModelColumn<int> invalidaciones;
        Gtk::TreeModelColumn<int> datos_transmitidos;
    };
    ColumnasPE columnas_pe;

    // Columnas para el bus
    class ColumnasBus : public Gtk::TreeModel::ColumnRecord {
    public:
        ColumnasBus();
        Gtk::TreeModelColumn<int> invalidaciones;
        Gtk::TreeModelColumn<int> read_request;
        Gtk::TreeModelColumn<int> write_request;
        Gtk::TreeModelColumn<int> instrucciones_procesadas;
    };
    ColumnasBus columnas_bus;

    // Métodos para cargar datos
    void cargar_datos_pe();
    void cargar_datos_bus();
};



#endif // GUI_H