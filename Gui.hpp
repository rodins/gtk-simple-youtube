// Gui.hpp
#include <string>
#include <iostream>
#include <gtk/gtk.h>

using namespace std;

class Gui {
    public:
    Gui() {
		const string PROG_NAME("Simple youtube");
        GtkWidget *window;
        GtkWidget *toolbar;
        GtkWidget *entry;
        GtkWidget *vbox;
        
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
		
		// Toolbar
		toolbar = gtk_toolbar_new();
		gtk_toolbar_set_style(GTK_TOOLBAR(toolbar), GTK_TOOLBAR_ICONS);
	    gtk_container_set_border_width(GTK_CONTAINER(toolbar), 2);
	    
	    entry = gtk_entry_new();
	    gtk_widget_set_tooltip_text(entry, "Search youtube");
		GtkToolItem *entryItem = gtk_tool_item_new();
		gtk_container_add(GTK_CONTAINER(entryItem), entry);
		g_signal_connect(entry,
                         "activate", 
                         G_CALLBACK(entryActivated), 
                         NULL);
	    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), entryItem, -1);
	    
	    gtk_widget_show_all(toolbar);
		
		vbox = gtk_vbox_new(FALSE, 1);
		gtk_box_pack_start(GTK_BOX(vbox), toolbar, FALSE, FALSE, 1);
		gtk_widget_show(vbox);
		gtk_container_add(GTK_CONTAINER(window), vbox);
		gtk_widget_show(window);
		gtk_main();
    }
    private:
    
    static void entryActivated( GtkWidget *widget, void* data) {
        string query(gtk_entry_get_text(GTK_ENTRY(widget)));
        if(!query.empty()) {
	        cout << query << endl;
	    }		  						  
    }
};