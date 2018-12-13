// ResultsModel.hpp

class ResultsModel {
    GtkListStore *resultsStore;
	GdkPixbuf *defaultPixbuf;
    public:
    ResultsModel(GtkListStore *store) {
		resultsStore = store;
		defaultPixbuf = IconsFactory::getBlankIcon();
	}
    
    void add(string title, string videoId, string imageHref) {
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
};