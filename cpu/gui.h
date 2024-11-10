#ifndef GUI_H
#define GUI_H

#include <gtkmm.h>

class Window : public Gtk::Window {
protected:
    Gtk::Box mBox;
    Gtk::Label label;
    
public:
    Window();
    ~Window();
};
#endif // GUI_H