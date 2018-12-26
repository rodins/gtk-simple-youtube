// CategoriesView.hpp

class CategoriesView {
    GtkWidget *spCategories;
    GtkWidget *swCategories;
    GtkWidget *btnCategoriesError;
    public:
    CategoriesView(GtkWidget *spCategories,
                   GtkWidget *swCategories,
                   GtkWidget *btnCategoriesError) {
	    this->spCategories = spCategories;
	    this->swCategories = swCategories;
	    this->btnCategoriesError = btnCategoriesError;				   
	}
	
	void showLoadingIndicator() {
		gtk_widget_show(spCategories);
		gtk_spinner_start(GTK_SPINNER(spCategories));
		gtk_widget_hide(swCategories);
		gtk_widget_hide(btnCategoriesError);
	}
	
	void showData() {
		gtk_widget_hide(spCategories);
		gtk_spinner_stop(GTK_SPINNER(spCategories));
		gtk_widget_show(swCategories);
		gtk_widget_hide(btnCategoriesError);
	}
	
	void showError() {
		gtk_widget_hide(spCategories);
		gtk_spinner_stop(GTK_SPINNER(spCategories));
		gtk_widget_hide(swCategories);
		gtk_widget_show(btnCategoriesError);
	}
};
