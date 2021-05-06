/**
 * OSX Implementation
 */

#include "ofxNative.h"
#include <Cocoa/Cocoa.h>
#include <pthread/pthread.h>
#include <Foundation/Foundation.h>

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

void ofxNative::openUrl( string url_s ){
	NSString * url_ns = [NSString stringWithUTF8String:url_s.c_str()];
	NSURL * url = [NSURL URLWithString:url_ns];
	[[NSWorkspace sharedWorkspace] openURL:url];
}


void ofxNative::maximizeWindow( ofAppGLFWWindow & window ){
	NSWindow * cocoaWindow = (NSWindow*)window.getCocoaWindow();
	[cocoaWindow setFrame:[[NSScreen mainScreen] visibleFrame] display:YES];
}


void ofxNative::setMinimumWindowSize( ofAppGLFWWindow & window, int minWidth, int minHeight ){
	NSWindow * cocoaWindow = (NSWindow*)window.getCocoaWindow();
	[cocoaWindow setMinSize:NSMakeSize(minWidth,minHeight)];
}

void ofxNative::setWindowAlwaysOnTop(ofAppBaseWindow & window, bool onTop){
		NSWindow * cocoaWindow = (NSWindow*)window.getCocoaWindow();
		[cocoaWindow setLevel:onTop?(NSFloatingWindowLevel):(NSNormalWindowLevel)];
}


void ofxNative::setMousePositionRelativeToWindow( ofVec2f pos ){
	ofVec2f cgPos =  pos + ofVec2f(ofGetWindowPositionX(),ofGetWindowPositionY());
	CGWarpMouseCursorPosition(CGPointMake(cgPos.x, cgPos.y));
	// additionally fake the event for OF
	if(ofGetMousePressed()){
		int buttonIdx = 0;
		for( int i = 0; i < 30; i++){
			if(ofGetMousePressed(i)){
				buttonIdx = i;
				break;
			}
		}
		ofEvents().notifyMouseDragged(pos.x, pos.y, buttonIdx);
	}
	else{
		ofEvents().notifyMouseMoved(pos.x, pos.y);
	}
}

// thread naming code from https://stackoverflow.com/a/23899379/347508

void ofxNative::setThreadName(const string & name){
	pthread_setname_np(name.c_str());
}


bool ofxNative::canShowConsole() {
	return false; 
}

bool ofxNative::getConsoleVisible() {
	return false; 
}

void ofxNative::setConsoleVisible(bool show) {
	// do nothing, because we can't
}

std::string ofxNative::getSystemDataFolder(){
	NSArray *paths = NSSearchPathForDirectoriesInDomains(NSApplicationSupportDirectory, NSUserDomainMask, YES);
	NSString *appName=[[[NSBundle mainBundle] infoDictionary]  objectForKey:(id)kCFBundleIdentifierKey];
	NSString *applicationSupportDirectory = [[paths firstObject] stringByAppendingPathComponent:appName];
	const char * absPath = [applicationSupportDirectory UTF8String];
	return string(absPath);
}

std::string ofxNative::getSystemDocumentsFolder(){
	NSArray * paths = NSSearchPathForDirectoriesInDomains (NSDocumentDirectory, NSUserDomainMask, YES);
	NSString * documentsDirectory = [paths objectAtIndex:0];
	const char * absPath = [documentsDirectory UTF8String];
	return string(absPath);
}
