// CategoriesNet.hpp

class CategoriesNet {
    CategoriesParser *parser;
    string API_KEY;
    
    CURL *curl_handle;
    
    string API_DOMAIN;
    string PART;
    string KEY;
    string FIELDS;
    public:
    
    CategoriesNet(CategoriesParser *parser, string apiKey) {
		this->parser = parser;
		
		API_DOMAIN = "https://www.googleapis.com/youtube/v3/videoCategories?";
		PART = "part=snippet";
		FIELDS = "&fields=items(id,snippet(assignable,title))";
		KEY = "&key=";
		
		API_KEY = KEY + apiKey;
		
		/* init the curl session */
		curl_handle = curl_easy_init();	
		if(curl_handle) {
			/* remove crash bug */
			curl_easy_setopt(curl_handle, CURLOPT_NOSIGNAL, 1);
			
			/* send all data to this function */		
			curl_easy_setopt(curl_handle, 
			                 CURLOPT_WRITEFUNCTION, 
			                 CategoriesNet::writer);
			curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1L);	
			curl_easy_setopt(curl_handle,
			                 CURLOPT_WRITEDATA, 
			                 parser);
		}
	}
	
	~CategoriesNet() {
		/* cleanup curl stuff */
	    curl_easy_cleanup(curl_handle);
	}
	
	CURLcode getDataFromNet() {
		string url = buildUrl();
		//cout << url << endl;
		CURLcode res;		
		if(curl_handle) {		    
		    /* set url to get here */
			curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());			
			                 			
			/* get it */
			res = curl_easy_perform(curl_handle);
			
			if(res == CURLE_OK) {
				parser->parse();
			}
		}
		return res;
	}
	
	private:
	
	string buildUrl() {
		string urlValues = PART+FIELDS+API_KEY;
		//TODO: set locale
		return API_DOMAIN + urlValues;
	}
	
	static int writer(char *data, size_t size, size_t nmemb, CategoriesParser *parser) {   
		if(parser == NULL) {
			return 0;
		}
		
	    parser->append(data);
	    return size*nmemb;
	}
};
