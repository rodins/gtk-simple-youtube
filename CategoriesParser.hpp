//CategoriesParser.hpp

class CategoriesParser {
	//CategoriesModel *model;
    string json;
    public:
    /*CategoriesParser(CategoriesModel *model) {
		this->model = model;
	}*/
	
	void append(string data) {
		json += data;
	}
	
	void parse() {
		cout << json << endl;
	}
};
