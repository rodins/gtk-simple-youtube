// ResultsModel.hpp

class ResultsModel {
    GtkListStore *store;
	GdkPixbuf *defaultPixbuf;
	map<string, GdkPixbuf*> *imagesCache;
    public:
    ResultsModel(GtkListStore *store, map<string, GdkPixbuf*> *imagesCache) {
		this->store = store;
		this->imagesCache = imagesCache;
		defaultPixbuf = IconsFactory::getBlankIcon();
	}
	
	void clear() {
		gtk_list_store_clear(store);
	}
    
    void add(string title, string videoId, string imageHref) {
		static GtkTreeIter iter;
		static GdkPixbuf *pixbuf;
		
		if(imagesCache->count(imageHref) > 0) {
			pixbuf = imagesCache->operator[](imageHref);
		}else {
			pixbuf = defaultPixbuf;
		}
		
		gtk_list_store_append(store, &iter);
        gtk_list_store_set(store,
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
