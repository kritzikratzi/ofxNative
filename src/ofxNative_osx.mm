#ifdef __APPLE__
/**
 * OSX Implementation
 */

#include "ofxNative.h"
#include <Cocoa/Cocoa.h>
#include <pthread/pthread.h>
#include <Foundation/Foundation.h>

using namespace ofxNative;

static void restoreAppWindowFocus();


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

void ofxNative::setWindowRepresentedFilename(ofAppBaseWindow & window, const string & title, const string & filename ){
	NSWindow * cocoaWindow = (NSWindow*)(window.getCocoaWindow());
	if(filename == ""){
		[cocoaWindow setRepresentedFilename:@""];
		[cocoaWindow setTitle:[NSString stringWithUTF8String:title.c_str()]];
	}
	else{
		NSString * file = [NSString stringWithUTF8String:filename.c_str()];
		NSString * fname = [file pathComponents].lastObject;
		[cocoaWindow setTitle:fname];
		[cocoaWindow setRepresentedFilename:file];
	}
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
	string absPath([applicationSupportDirectory UTF8String]);
	ofDirectory dir(absPath);
	if(!dir.exists()) dir.create();
	return absPath;
}

std::string ofxNative::getSystemDocumentsFolder(){
	NSArray * paths = NSSearchPathForDirectoriesInDomains (NSDocumentDirectory, NSUserDomainMask, YES);
	NSString * documentsDirectory = [paths objectAtIndex:0];
	const char * absPath = [documentsDirectory UTF8String];
	return string(absPath);
}

std::string ofxNative::getTempFolder(){
	return string( NSTemporaryDirectory().UTF8String );
}

// originally from openFrameworks.
// changed only to include file extensions
ofFileDialogResult ofxNative::systemLoadDialog(std::string windowTitle, bool bFolderSelection, std::string defaultPath, std::vector<std::string> extensions){
	ofFileDialogResult results;
	NSOpenGLContext *context = [NSOpenGLContext currentContext];

	NSOpenPanel * loadDialog = [NSOpenPanel openPanel];
	[loadDialog setAllowsMultipleSelection:NO];
	[loadDialog setCanChooseDirectories:bFolderSelection];
	[loadDialog setCanChooseFiles:!bFolderSelection];
	[loadDialog setResolvesAliases:YES];
	if(extensions.size()>0){
		NSMutableArray * nsextensions = [[NSMutableArray alloc] init];
		for(std::string ext : extensions){
			[nsextensions addObject:[NSString stringWithUTF8String:ext.c_str()]];
		}
		[loadDialog setAllowedFileTypes:nsextensions];
	}
	if(!windowTitle.empty()) {
		[loadDialog setTitle:[NSString stringWithUTF8String:windowTitle.c_str()]];
	}

	if(!defaultPath.empty()) {
		NSString * s = [NSString stringWithUTF8String:defaultPath.c_str()];
		s = [[s stringByExpandingTildeInPath] stringByResolvingSymlinksInPath];
		NSURL * defaultPathUrl = [NSURL fileURLWithPath:s];
		[loadDialog setDirectoryURL:defaultPathUrl];
	}

	NSInteger buttonClicked = [loadDialog runModal];
	[context makeCurrentContext];

	restoreAppWindowFocus();

	if(buttonClicked == NSFileHandlingPanelOKButton) {
		NSURL * selectedFileURL = [[loadDialog URLs] objectAtIndex:0];
		results.filePath = string([[selectedFileURL path] UTF8String]);
	}

	if( results.filePath.length() > 0 ){
		results.bSuccess = true;
		results.fileName = ofFilePath::getFileName(results.filePath);
	}
	
	return results;
}

// originally from openFrameworks.
// changed only to include file extensions
ofFileDialogResult ofxNative::systemSaveDialog(string defaultName, string messageName, std::vector<std::string> extensions){

	ofFileDialogResult results;

	//----------------------------------------------------------------------------------------
	//------------------------------------------------------------------------------       OSX
	//----------------------------------------------------------------------------------------
	NSSavePanel * saveDialog = [NSSavePanel savePanel];
	NSOpenGLContext *context = [NSOpenGLContext currentContext];
	[saveDialog setMessage:[NSString stringWithUTF8String:messageName.c_str()]];
	[saveDialog setNameFieldStringValue:[NSString stringWithUTF8String:defaultName.c_str()]];
	if(extensions.size()>0){
		NSMutableArray * nsextensions = [[NSMutableArray alloc] init];
		for(std::string ext : extensions){
			[nsextensions addObject:[NSString stringWithUTF8String:ext.c_str()]];
		}
		[saveDialog setAllowedFileTypes:nsextensions];
	}
	NSInteger buttonClicked = [saveDialog runModal];
	restoreAppWindowFocus();
	[context makeCurrentContext];

	if(buttonClicked == NSFileHandlingPanelOKButton){
		results.filePath = string([[[saveDialog URL] path] UTF8String]);
	}

	if( results.filePath.length() > 0 ){
		results.bSuccess = true;
		results.fileName = ofFilePath::getFileName(results.filePath);
	}

	return results;
}


// originally from openFrameworks.
// changed only to include file extensions
static void restoreAppWindowFocus(){
	NSWindow * appWindow = (NSWindow *)ofGetCocoaWindow();
	if(appWindow) {
		[appWindow makeKeyAndOrderFront:nil];
	}
}

#endif
