#ifdef __linux__

#include "ofxNative.h"
#include <pthread.h>
#include <cstdlib>

using namespace ofxNative;

string getExecutableName() {
	char exe[1024];
	int len = readlink("/proc/self/exe",exe,sizeof(exe)-1);

	if (len < 0) 
		return "ofApp";

	exe[len] = 0;
	char* basename = g_path_get_basename(exe);
	string name(basename);
	free(basename);

	return name;
}


void ofxNative::showFile(string filename) {
	pid_t pid = fork();
	if (pid == 0) {
		char* dir = g_path_get_dirname(filename.c_str());
		execl("/usr/bin/xdg-open", "xdg-open", dir, (char *)0);
		free(dir);
		exit(1);
	}
}


void ofxNative::openFile(string filename){
	pid_t pid = fork();
	if (pid == 0) {
		execl("/usr/bin/xdg-open", "xdg-open", filename.c_str(), (char *)0);
		exit(1);
	}
}

void ofxNative::openUrl(string url){
	pid_t pid = fork();
	if (pid == 0) {
		execl("/usr/bin/xdg-open", "xdg-open", url.c_str(), (char *)0);
		exit(1);
	}
}


void ofxNative::maximizeWindow( ofAppGLFWWindow & window ){
	// not implemented
	cerr << "ofxNative::maximizeWindow() not implemented for Linux" << endl;
}


void ofxNative::setMinimumWindowSize( ofAppGLFWWindow & window, int minWidth, int minHeight ){
	// not implemented
	cerr << "ofxNative::setMinimumWindowSize() not implemented for Linux" << endl;
}


void ofxNative::setWindowAlwaysOnTop(ofAppBaseWindow & window, bool onTop){
	// not implemented
	cerr << "ofxNative::setWindowAlwaysOnTop() not implemented for Linux" << endl;
}


void ofxNative::setMousePositionRelativeToWindow( ofVec2f pos ){
	// not implemented
	cerr << "ofxNative::setMousePositionRelativeToWindow() not implemented for Linux" << endl;
}


void ofxNative::setThreadName(const string & name){
	pthread_setname_np(pthread_self(), name.c_str());
}

bool ofxNative::canShowConsole() {
	return false;
}

bool ofxNative::getConsoleVisible() {
	// not implemented
	cerr << "ofxNative::getConsoleVisible() not implemented for Linux" << endl;
	return false;
}

void ofxNative::setConsoleVisible(bool show) {
	// not implemented
	cerr << "ofxNative::setConsoleVisible() not implemented for Linux" << endl;
}

string getSystemDataPrefix() {
	if (const char* data = getenv("XDG_DATA_HOME"))
		return std::string(data) + "/";

	if (const char* home = getenv("HOME"))
		return std::string(home) + "/.local/share/";

	return "";
}

string ofxNative::getSystemDataFolder() {
	string prefix = getSystemDataPrefix();
	if (prefix.empty())
		return ofToDataPath("");

	string dirname = prefix + getExecutableName();
	ofDirectory(dirname).create();
	return dirname;
}

#endif
