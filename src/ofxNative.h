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
	
	// moves the mouse to a location relative the window coordinates
	// eg setMousePositionRelativeToWindow({10+ofGetMouseX(),ofGetMouseY()}) moves it 10px to the right.
	// funny issues might arise with retina screens.
	void setMousePositionRelativeToWindow( ofVec2f pos );
}