#ifdef __linux__

#include "ofxNative.h"
using namespace ofxNative;

std::string getExecutablePath() {
	return std::string(getcwd(nullptr, 0));
}

void ofxNative::showFile(string filename) {
	pid_t pid = fork();
	if (pid == 0) {
		execl("/usr/bin/xdg-open", "xdg-open", filename.c_str(), (char *)0);
		exit(1);
	}
}


void ofxNative::openFile( string filename ){
	pid_t pid = fork();
	if (pid == 0) {
		execl("/usr/bin/xdg-open", "xdg-open", filename.c_str(), (char *)0);
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


void ofxNative::setThreadName( const string & name){
	// not implemented
	cerr << "ofxNative::setThreadName() not implemented for Linux" << endl;
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

std::string ofxNative::getSystemDataFolder() {
	return ofToDataPath("");
}

#endif
