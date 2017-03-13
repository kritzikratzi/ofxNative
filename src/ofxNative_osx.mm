/**
 * OSX Implementation
 */

#include "ofxNative.h"
#include <Cocoa/Cocoa.h>

using namespace ofxNative;

void ofxNative::showFile( string path ){
	NSString * file = [NSString stringWithUTF8String:path.c_str()];
	NSURL * url = [NSURL fileURLWithPath:file];
	NSArray *fileURLs = [NSArray arrayWithObjects:url, nil];
	[[NSWorkspace sharedWorkspace] activateFileViewerSelectingURLs:fileURLs];
}



void ofxNative::openFile( string path ){
	NSString * file = [NSString stringWithUTF8String:path.c_str()];
	[[NSWorkspace sharedWorkspace] openFile:file];
}


void ofxNative::setCursor(ofxNative::CursorType cursor){
	switch(cursor){
		case kCursorTypeDefault:
			[[NSCursor arrowCursor] set];
			break;
		case kCursorTypeHand:
			[[NSCursor pointingHandCursor] set];
			break;
	}
}
