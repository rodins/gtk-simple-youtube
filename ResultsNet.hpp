// ResultsNet.hpp

class ResultsNet {
    ResultsParser *parser;
    string query;
    string API_KEY;
    
    CURL *curl_handle;
    
    string API_DOMAIN;
    string SEARCH;
    string PART;
    string MAX_RESULTS;
    string TYPE;
    string FIELDS;
    string KEY;
    string PAGE_TOKEN;
    string QUERY;
    
    string pageToken;
    
    public:
    ResultsNet(ResultsParser *parser, string apiKey) {
	    API_DOMAIN = "https://www.googleapis.com/youtube/v3/";
        SEARCH = "search?";
        PART = "part=snippet";
        MAX_RESULTS = "&maxResults=15";
        TYPE = "&type=video";
        FIELDS = "&fields=items(id/videoId,snippet(thumbnails/default/url,title)),nextPageToken";
        KEY = "&key=";
        PAGE_TOKEN = "&pageToken=";
        QUERY = "&q=";
		
		this->parser = parser;
		API_KEY = KEY + apiKey;
		
		/* init the curl session */
		curl_handle = curl_easy_init();	
		if(curl_handle) {
			/* remove crash bug */
			curl_easy_setopt(curl_handle, CURLOPT_NOSIGNAL, 1);
			
			/* send all data to this function */		
			curl_easy_setopt(curl_handle, 
			                 CURLOPT_WRITEFUNCTION, 
			                 ResultsNet::results_writer);
			curl_easy_setopt(curl_handle, CURLOPT_FOLLOWLOCATION, 1L);	
			curl_easy_setopt(curl_handle,
			                 CURLOPT_WRITEDATA, 
			                 parser);
		}
	}
	
	~ResultsNet() {
		/* cleanup curl stuff */
	    curl_easy_cleanup(curl_handle);
	}
		
	void setQuery(string &query) {
		pageToken = "";
		this->query = query;
	}
	
	bool isPaging() {
		return !pageToken.empty();
	}
	
	CURLcode getResultsFromNet() {
		string url = buildSearchListUrl();
		//cout << url << endl;
		CURLcode res;		
		if(curl_handle) {		    
		    /* set url to get here */
			curl_easy_setopt(curl_handle, CURLOPT_URL, url.c_str());			
			                 			
			/* get it */
			res = curl_easy_perform(curl_handle);
			
			if(res == CURLE_OK) {
				parser->parse();
				gdk_threads_enter();
				pageToken = parser->getNextPageToken();
				gdk_threads_leave();
			}
		}
		return res;
	}
	
	private:
	
	string buildSearchListUrl() {
		string urlValues = PART+MAX_RESULTS+TYPE+FIELDS+API_KEY;
		if(!query.empty()) {
			char *queryEscaped = curl_easy_escape(curl_handle,
			                                      query.c_str(),
			                                      query.size());
			urlValues += QUERY + string(queryEscaped);
		}
		if(!pageToken.empty()) {
			urlValues += PAGE_TOKEN + pageToken;
		}
		
		return API_DOMAIN + SEARCH + urlValues;
	}
	
	static int results_writer(char *data, 
	                          size_t size, 
	                          size_t nmemb,
	                          ResultsParser *parser)
	{   
		if(parser == NULL) {
			return 0;
		}
		
	    parser->append(data);
	    return size*nmemb;
	}
};
