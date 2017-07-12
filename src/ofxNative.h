/**
 * Native utils for openFramworks
 *
 */

#include "ofMain.h"

namespace ofxNative{
	
	// shows a file in finder/explorer
	void showFile( string path );
	
	// opens a file with the default application
	void openFile( string path );
	
	// maximizes a GLFWWindow on the current screen
	void maximizeWindow( ofAppGLFWWindow & window );
	
	// sets the minimum size for a GLFWWindow
	void setMinimumWindowSize( ofAppGLFWWindow & window, int minWidth, int minHeight );
	
}