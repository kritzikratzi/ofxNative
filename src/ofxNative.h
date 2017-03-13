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
	
	
	enum CursorType{
		kCursorTypeDefault,
		kCursorTypeHand
	};
	
	// sets the cursor for the current window
	void setCursor(CursorType cursor);
}