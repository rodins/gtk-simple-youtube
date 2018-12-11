#include "FileUtils.hpp"
#include "Gui.hpp"

int main(int argc, char *argv[]) {
	string apiKey = FileUtils::getApiKeyFromFile();
	Gui gui(apiKey);
}
