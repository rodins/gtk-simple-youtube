// ResultsParser.hpp

class ResultsParser {
    //ResultsModel *model;
    string json;
    public:
    /*ResultsParser(ResultsModel *model) {
        this->model = model;
    }*/
    
    void append(string data) {
		json += data;
	}
	
	void parse() {
		cout << json << endl;
	}
};
