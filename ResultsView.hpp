// ResultsView.hpp

class ResultsView {
    GtkWidget *swResults;
	GtkWidget *spResults;
	GtkWidget *hbResultsError;
	GtkWidget *vbox;
	public:
	ResultsView(GtkWidget *swResults, 
	            GtkWidget *spResults,
	            GtkWidget *hbResultsError,
	            GtkWidget *vbox) {
	    this->swResults = swResults;
	    this->spResults = spResults;
	    this->hbResultsError = hbResultsError;
	    this->vbox = vbox;				
	}
	
	void showLoadingIndicator(bool isPaging) {
		gtk_box_set_child_packing(
		    GTK_BOX(vbox),
		    spResults,
		    !isPaging,
		    FALSE,
		    1,
		    GTK_PACK_START);
		gtk_widget_show(spResults);
		gtk_spinner_start(GTK_SPINNER(spResults));
		gtk_widget_set_visible(swResults, isPaging);
		gtk_widget_hide(hbResultsError);
	}
	
	void showData() {
		gtk_widget_hide(spResults);
		gtk_spinner_stop(GTK_SPINNER(spResults));
		gtk_widget_show(swResults);
		gtk_widget_hide(hbResultsError);
	}
	
	void showError(bool isPaging) {
		gtk_box_set_child_packing(
		    GTK_BOX(vbox),
		    hbResultsError,
		    !isPaging,
		    FALSE,
		    1,
		    GTK_PACK_START);
		gtk_widget_hide(spResults);
		gtk_spinner_stop(GTK_SPINNER(spResults));
		gtk_widget_set_visible(swResults, isPaging);
		gtk_widget_show(hbResultsError);
	}
				
};
