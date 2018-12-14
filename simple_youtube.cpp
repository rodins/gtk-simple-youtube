#include <string>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdlib.h>
#include <map>
#include <set>

#include <glib.h>
#include <glib/gstdio.h>
#include <gtk/gtk.h>
#include <curl/curl.h>
#include <gdk-pixbuf/gdk-pixbuf.h>

using namespace std;

#include "FileUtils.hpp"
#include "Gui.hpp"

int main(int argc, char *argv[]) {
	string apiKey = FileUtils::getApiKeyFromFile();
	Gui gui(apiKey);
}
