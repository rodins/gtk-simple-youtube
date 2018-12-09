// Gui.hpp
#include <string>
#include <iostream>
#include <gtk/gtk.h>

using namespace std;

#include "ColumnsEnum.hpp"
#include "IconsFactory.hpp"

class Gui {
	GtkWidget *swResults;
	GtkWidget *spResults;
	GtkWidget *hbResultsError;
	GtkListStore *resultsStore;
	GdkPixbuf *defaultPixbuf;
    public:
    Gui() {
		const string PROG_NAME("Simple youtube");
		const int IV_RESULT_ITEM_WIDTH = 180;
		const int SPINNER_SIZE = 32;
		
		defaultPixbuf = IconsFactory::getBlankIcon();
		
        GtkWidget *window;
        GtkWidget *toolbar;
        GtkWidget *entry;
        
        GtkWidget *ivResults;
        GtkWidget *btnResultsError;
        
        GtkWidget *vbox;
        
        gtk_init(0, NULL);
        
        window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
	    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	    gtk_window_set_title(GTK_WINDOW(window), PROG_NAME.c_str());
	    gtk_container_set_border_width(GTK_CONTAINER(window), 5);
	    // Add application icon
	    GdkPixbuf *appIcon = IconsFactory::getAppIcon();
		gtk_window_set_icon(GTK_WINDOW(window), appIcon);
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
                         this);
	    gtk_toolbar_insert(GTK_TOOLBAR(toolbar), entryItem, -1);
	    gtk_widget_show_all(toolbar);
	    
	    // Results icon view
		// set model to ivResults
	    resultsStore = gtk_list_store_new(
		     ICON_NUM_COLS,   // Number of columns
		     GDK_TYPE_PIXBUF, // Image poster
		     G_TYPE_STRING,   // Title
		     G_TYPE_STRING,   // Video id
		     G_TYPE_STRING    // Image link
	    );
	    
	    ivResults = gtk_icon_view_new_with_model(GTK_TREE_MODEL(resultsStore));
	    gtk_icon_view_set_pixbuf_column(GTK_ICON_VIEW(ivResults), ICON_IMAGE_COLUMN);                                                  
	    gtk_icon_view_set_text_column(GTK_ICON_VIEW(ivResults), ICON_TITLE_COLUMN);
	    gtk_icon_view_set_item_width(GTK_ICON_VIEW(ivResults), IV_RESULT_ITEM_WIDTH);
	    
	    g_signal_connect(ivResults, 
                         "item-activated", 
                         G_CALLBACK(resultActivated), 
                         NULL);
	    
	    swResults = createScrolledWindow();
	    gtk_container_add(GTK_CONTAINER(swResults), ivResults);
	    gtk_widget_show_all(swResults);
	    
	    spResults = gtk_spinner_new();
	    gtk_widget_set_size_request(spResults, SPINNER_SIZE, SPINNER_SIZE);
	    
	    btnResultsError = gtk_button_new_with_label("Repeat");
	    gtk_widget_show(btnResultsError);
	    g_signal_connect(btnResultsError,
                         "clicked",
                         G_CALLBACK(btnResultsErrorClicked),
                         NULL);
	    hbResultsError = gtk_hbox_new(false, 1);
	    gtk_box_pack_start(GTK_BOX(hbResultsError), 
	                       btnResultsError, 
	                       true, 
	                       false, 
	                       10);
		
		vbox = gtk_vbox_new(false, 1);
		gtk_box_pack_start(GTK_BOX(vbox), toolbar, false, false, 1);
		gtk_box_pack_start(GTK_BOX(vbox), hbResultsError, true, false, 1);
		gtk_box_pack_start(GTK_BOX(vbox), swResults, true, true, 1);
		gtk_box_pack_start(GTK_BOX(vbox), spResults, true, false, 1);
		
		gtk_widget_show(vbox);
		gtk_container_add(GTK_CONTAINER(window), vbox);
		gtk_widget_show(window);
		gtk_main();
    }
    
    void showResultsLoadingIndicator() {
		gtk_widget_show(spResults);
		gtk_spinner_start(GTK_SPINNER(spResults));
		gtk_widget_hide(swResults);
		gtk_widget_hide(hbResultsError);
	}
	
	void showResultsData() {
		gtk_widget_hide(spResults);
		gtk_spinner_stop(GTK_SPINNER(spResults));
		gtk_widget_show(swResults);
		gtk_widget_hide(hbResultsError);
	}
	
	void showResultsError() {
		gtk_widget_hide(spResults);
		gtk_spinner_stop(GTK_SPINNER(spResults));
		gtk_widget_hide(swResults);
		gtk_widget_show(hbResultsError);
	}
	
	void addToResultsModel(string title, string videoId, string imageHref) {
		static GtkTreeIter iter;
		static GdkPixbuf *pixbuf;
		
		gtk_list_store_append(resultsStore, &iter);
		
		/*if(imagesCache->count(imageHref) > 0) {
			pixbuf = imagesCache->operator[](imageHref);
		}else {
			pixbuf = defaultPixbuf;
		}*/
		
		pixbuf = defaultPixbuf;
		
        gtk_list_store_set(resultsStore, 
                           &iter,
                           ICON_IMAGE_COLUMN, 
                           pixbuf,
                           ICON_TITLE_COLUMN, 
                           title.c_str(),
                           ICON_VIDEO_ID, 
                           videoId.c_str(),
                           ICON_IMAGE_LINK, 
                           imageHref.c_str(), 
                           -1);
	}
	
	void startResultsTaskForQuery(string query) {
		cout << "Not yet implemented" << endl;
	}
	
    private:
    
    GtkWidget* createScrolledWindow() {
		GtkWidget* scrolledWindow = gtk_scrolled_window_new(NULL, NULL);
	    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolledWindow),
	                                   GTK_POLICY_AUTOMATIC, 
	                                   GTK_POLICY_AUTOMATIC);
	    gtk_scrolled_window_set_shadow_type(GTK_SCROLLED_WINDOW(scrolledWindow),
	                                        GTK_SHADOW_ETCHED_IN);
	    return scrolledWindow;
	}
    
    static void entryActivated(GtkWidget *widget, Gui *gui) {
        string query(gtk_entry_get_text(GTK_ENTRY(widget)));
        if(!query.empty()) {
	        gui->addToResultsModel(query, query, query);
	    }		  						  
    }
    
    static void btnResultsErrorClicked(GtkWidget *widget, gpointer data) {
		cout << "Button results error clicked" << endl;
	}
	
	static void resultActivated(GtkIconView *icon_view, GtkTreePath *path, gpointer data) {
	                    
		// Get model from ivResults
		GtkTreeModel *model = gtk_icon_view_get_model(icon_view);
		
		// Get iter from path
		GtkTreeIter iter;
		gtk_tree_model_get_iter(model, &iter, path);
		
		// Get title and href value from iter
		gchar *resultTitle = NULL;
		gchar *videoId = NULL;
		gchar *imageHref = NULL;
	
		gtk_tree_model_get(model,
		                   &iter,
		                   ICON_TITLE_COLUMN,
		                   &resultTitle,
		                   ICON_VIDEO_ID,
		                   &videoId,
		                   ICON_IMAGE_LINK,
		                   &imageHref,
		                   -1);
		                   
		cout << resultTitle << endl;
		cout << videoId << endl;
		cout << imageHref << endl;
		
		g_free(resultTitle);
		g_free(videoId);
		g_free(imageHref);
	}
};
