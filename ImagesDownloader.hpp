//ImagesDownloader.hpp

//Convert int to string
#define SSTR( x ) static_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()

struct ArgsStruct {
	GtkTreeIter iter;
	GtkListStore *store;
};

class ImagesDownloader {
    GtkWidget *ivResults;
    GThreadPool *imagesThreadPool;
    set<int> *imageIndices;
    map<string, GdkPixbuf*> *imagesCache;
    
    public:
    
    ImagesDownloader(GtkWidget *ivResults,
                     set<int> *imageIndices,
                     map<string, GdkPixbuf*> *imagesCache) {
		this->ivResults = ivResults;
		this->imageIndices = imageIndices;
		this->imagesCache = imagesCache;
		
		// GThreadPool for downloading images
	    imagesThreadPool = g_thread_pool_new(ImagesDownloader::imageDownloadTask,
	                                         this, 
	                                         -1,
	                                         FALSE,
	                                         NULL);
	    
	    g_signal_connect(ivResults, 
                         "expose-event",
                         G_CALLBACK(ImagesDownloader::iconViewExposed), 
                         this);
	}
	
	private:
	
	static size_t
	WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
	{
		size_t realsize = size * nmemb;
		GdkPixbufLoader *loader = (GdkPixbufLoader*)userp;
		gdk_pixbuf_loader_write(loader, (const guchar*)contents, realsize, NULL);
		return realsize;
	}
	
	static void imageDownloadTask(gpointer arg, gpointer arg1) {
		ImagesDownloader *imagesDownloader = (ImagesDownloader*)arg1;
		gint index = (gint)(long)arg;
		index--;
		
		gdk_threads_enter();
		GtkTreeModel* model = gtk_icon_view_get_model(
		     GTK_ICON_VIEW(imagesDownloader->ivResults)
		);
		gdk_threads_leave();
		
		string strIndex = SSTR(index);
		GtkTreeIter iter;
		if(gtk_tree_model_get_iter_from_string (
		    model,
		    &iter,
		    strIndex.c_str())) {
			// Get image link value from iter
	        gchar *imageLink = NULL;
	        gtk_tree_model_get(model, &iter, ICON_IMAGE_LINK, &imageLink, -1);
	        string link(imageLink);
	        g_free(imageLink);
	        
	        if(link == "") return;
	        
	        gdk_threads_enter();
	        int count = imagesDownloader->imagesCache->count(link);
	        gdk_threads_leave();
	        
		    if(count == 0) { 
				imagesDownloader->getPixbufFromUrl(link,
				                                   iter, 
				                                   index, 
				                                   GTK_LIST_STORE(model));
			}
		}
	}
	
	static void loaderAreaPrepared(GdkPixbufLoader *loader, gpointer user_data) {
		struct ArgsStruct *args = (struct ArgsStruct *)user_data;
		GdkPixbuf *pixbuf = gdk_pixbuf_loader_get_pixbuf(loader);
		gdk_threads_enter();
		gtk_list_store_set(args->store, &args->iter, ICON_IMAGE_COLUMN, pixbuf, -1);  
		gdk_threads_leave();
	}
	
	void getPixbufFromUrl(string url, GtkTreeIter iter, int index, GtkListStore* store) {
		CURL *curl_handle;
		CURLcode res;
		
		GdkPixbufLoader* loader = gdk_pixbuf_loader_new();
		 
		struct ArgsStruct args;
		args.iter = iter;
		args.store = store;
		g_signal_connect(loader,
		                 "area-prepared",
		                 G_CALLBACK(ImagesDownloader::loaderAreaPrepared),
		                 &args);
		
		/* init the curl session */ 
		curl_handle = curl_easy_init();
		
		/* remove crash bug */
		curl_easy_setopt(curl_handle, CURLOPT_NOSIGNAL, 1);
		
		/* specify URL to get */ 
		curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());
		
		/* send all data to this function  */ 
		curl_easy_setopt(curl_handle, 
		                 CURLOPT_WRITEFUNCTION, 
		                 ImagesDownloader::WriteMemoryCallback);
		
		/* we pass our 'chunk' struct to the callback function */ 
		curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, loader);
		
		/* some servers don't like requests that are made without a user-agent
		 field, so we provide one */ 
		curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
		
		/* get it! */ 
		res = curl_easy_perform(curl_handle);
		
		/* check for errors */ 
		if(res != CURLE_OK) {
			gdk_pixbuf_loader_close(loader, NULL); //close in case of error
			fprintf(stderr, "curl_easy_perform() failed: %s\n",
					curl_easy_strerror(res));
			// In case of error remove index from imageIndices 
	        // to have a chance to reload image
	        gdk_threads_enter();
	        imageIndices->erase(index);
	        gdk_threads_leave();
		}else { 
	        GError *error = NULL;
			gboolean ok = gdk_pixbuf_loader_close(loader, &error);
			if(!ok) {
		        fprintf(stderr, "On close: %s\n", error->message);
		        // In case of loader error do not reload image
		        // Do not erase index
		        //imageIndices->erase(index);
	            g_error_free(error);
			}else {
				gdk_threads_enter();
	            //Make copy of pixbuf to be able to free loader
				GdkPixbuf *pixbuf = GDK_PIXBUF(g_object_ref(gdk_pixbuf_loader_get_pixbuf(loader)));
				(*imagesCache)[url] = pixbuf;
				gtk_list_store_set(store, &iter, ICON_IMAGE_COLUMN, pixbuf, -1);
				gdk_threads_leave();
	        }
		}
		
		/* cleanup curl stuff */ 
		curl_easy_cleanup(curl_handle);
		
		g_object_unref(loader);
	}
	
	static gboolean iconViewExposed(GtkIconView *icon_view, 
	                                GdkEvent *event, 
	                                gpointer data) {
		ImagesDownloader *imageDownloader = (ImagesDownloader*)data;
		GtkTreePath *path1, *path2;
		if(gtk_icon_view_get_visible_range(icon_view, 
		                                   &path1,
		                                   &path2)) {
			gint *indices1 = gtk_tree_path_get_indices(path1);
			gint *indices2 = gtk_tree_path_get_indices(path2);
			gint indexDisplayFirst = indices1[0];
			gint indexDisplayLast = indices2[0];
			
		    // Downloading images for displayed items
			for(int i = indexDisplayFirst; i <= indexDisplayLast; i++) {
				if(imageDownloader->imageIndices->count(i) == 0) {
					imageDownloader->imageIndices->insert(i);
					g_thread_pool_push(imageDownloader->imagesThreadPool, 
			                           (gpointer)(long)(i+1),
			                           NULL);
				}	
			}		
			
			gtk_tree_path_free(path1);
			gtk_tree_path_free(path2);
		}
		return FALSE;
	}
};
