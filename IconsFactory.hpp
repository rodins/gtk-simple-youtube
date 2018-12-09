// IconsFactory.hpp

class IconsFactory {
	static string get_progdir() {
		char buff[PATH_MAX];
	    ssize_t len = readlink("/proc/self/exe", buff, sizeof(buff)-1);
	    if (len != -1) {
	        buff[len] = '\0';
	        return string(buff);
	    }
	    return "";
	}
	
	static GdkPixbuf* create_pixbuf(const gchar* filename) {
		string progpath = get_progdir();
		gchar* progdir = g_path_get_dirname(progpath.c_str());
		gchar* full_filename = g_build_filename(progdir, filename, NULL);
    
	    GdkPixbuf *pixbuf;
	    GError *error = NULL;
	    pixbuf = gdk_pixbuf_new_from_file(full_filename, &error);
	    
	    if (!pixbuf) {  
	        fprintf(stderr, "%s\n", error->message);
	        g_error_free(error); 
	    }
	    g_free(full_filename);
	    return pixbuf;
	}
	
	public:
	
    /*static GdkPixbuf* getLinkIcon() {
		return create_pixbuf("images/link_16.png");
	}
	
	static GdkPixbuf* getFolderIcon() {
		return create_pixbuf("images/folder_16.png");
	}*/
	
	static GdkPixbuf* getAppIcon() {
		return create_pixbuf("images/youtube.png");
	}
	
	static GdkPixbuf* getBlankIcon() {
		return create_pixbuf("images/blank_default.png");
	}
	
	/*static GdkPixbuf* getBookmarkIcon() {
		return create_pixbuf("images/bookmark_16.png");
	}
	
	static GdkPixbuf* getBookmarkIcon24() {
		return create_pixbuf("images/bookmark_24.png");
	}*/
};
