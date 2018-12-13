// FileUnitls.hpp

class FileUtils {
    public:
    static string get_progdir() {
		char buff[PATH_MAX];
	    ssize_t len = readlink("/proc/self/exe", buff, sizeof(buff)-1);
	    if (len != -1) {
	        buff[len] = '\0';
	        return string(buff);
	    }
	    return "";
	}
	
	static string getApiKeyFromFile() { 
		string progpath = get_progdir();
		gchar* progdir = g_path_get_dirname(progpath.c_str());
		gchar *filename = g_build_filename(progdir, "key.txt", NULL);
		ifstream in(filename);
		if(in) {
			string apiKey = string((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
			size_t newLine = apiKey.find('\n');
			if(newLine != string::npos) {
				apiKey.erase(newLine);
			}
			return apiKey;
		}
		return "";
	}
};
