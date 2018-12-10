// ResultsView.hpp

class ResultsView {
    GtkWidget *swResults;
	GtkWidget *spResults;
	GtkWidget *hbResultsError;
	public:
	ResultsView(GtkWidget *swResults, 
	            GtkWidget *spResults,
	            GtkWidget *hbResultsError) {
	    this->swResults = swResults;
	    this->spResults = spResults;
	    this->hbResultsError = hbResultsError;				
	}
	
	void showLoadingIndicator() {
		gtk_widget_show(spResults);
		gtk_spinner_start(GTK_SPINNER(spResults));
		gtk_widget_hide(swResults);
		gtk_widget_hide(hbResultsError);
	}
	
	void showData() {
		gtk_widget_hide(spResults);
		gtk_spinner_stop(GTK_SPINNER(spResults));
		gtk_widget_show(swResults);
		gtk_widget_hide(hbResultsError);
	}
	
	void showError() {
		gtk_widget_hide(spResults);
		gtk_spinner_stop(GTK_SPINNER(spResults));
		gtk_widget_hide(swResults);
		gtk_widget_show(hbResultsError);
	}
				
};
