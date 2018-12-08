// Gui.hpp
#include <string>
#include <gtk/gtk.h>

using namespace std;

class Gui {
    public:
    Gui() {
		const string PROG_NAME("Simple youtube");
        GtkWidget *window;
        
        gtk_init(0, NULL);
        
        window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	    gtk_window_set_title(GTK_WINDOW(window), PROG_NAME.c_str());
	    gtk_container_set_border_width(GTK_CONTAINER(window), 5);
	    // TODO: Add application icon
	    //icon = IconsFactory::getAppIcon();
		//gtk_window_set_icon(GTK_WINDOW(window), icon);
		
		gtk_window_set_default_size(GTK_WINDOW(window), 700, 400);
		gtk_widget_show(window);
		gtk_main();
    }
};