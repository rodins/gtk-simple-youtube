// ResultsParser.hpp

class ResultsParser {
    ResultsModel *model;
    string json;
    string nextPageToken;
    public:
    ResultsParser(ResultsModel *model) {
        this->model = model;
    }
    
    void append(string data) {
		json += data;
	}
	
	string getNextPageToken() {
		return nextPageToken;
	}
	
	void parse() {
		nextPageToken = parseItem(json, "nextPageToken", ",");
		string queryId = "\"id\":";
		size_t id = json.find(queryId);
		size_t nextId;
		while(id != string::npos) {
			nextId = json.find(queryId, id + queryId.size());
			string itemJson;
			if(nextId != string::npos) {
				size_t idLength = nextId - id;
				itemJson = json.substr(id, idLength);
			}else {
				itemJson = json.substr(id);
			}
			string videoId = parseItem(itemJson, "videoId", "\n");
			string title = parseItem(itemJson, "title", ",");
			replace(title, "\\", ""); // replace escape character in title
			string imageUrl = parseItem(itemJson, "url", "\n");
			gdk_threads_enter();
			model->add(title, videoId, imageUrl);
			gdk_threads_leave();
			id = nextId;
		}
		json = "";
	}
	
	private:
	
	void replace(string &title, const string to_replace, const string replace_with) {
		size_t entity_found = title.find(to_replace);
		while(entity_found != string::npos){
		    title.replace(entity_found, to_replace.length(),replace_with);
			entity_found = title.find(to_replace, entity_found+1);// поиск следующего элемента в строке
		}
	}
	
	string parseItem(string js, string queryBegin, string queryEnd) {
		const int BEGIN_ADD = 4; // ": " = four chars
		const int END_MINUS = 1; // " = one char
		size_t begin = js.find(queryBegin);
		size_t end = js.find(queryEnd, begin);  
		if(begin != string::npos && end != string::npos) {
			int shift = queryBegin.size() + BEGIN_ADD;
			size_t length = end - END_MINUS - begin - shift;
			return js.substr(begin + shift, length);
		}
		return "";
	}
};
