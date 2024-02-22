#include "ofxNative.h"

#if defined(TARGET_OS_IOS) && TARGET_OS_IOS
/**
 * IOS Implementation
 */

#include "ofxiOSExtras.h"

#include <pthread/pthread.h>
#include <Foundation/Foundation.h>

using namespace ofxNative;

void ofxNative::showFile( string path ){
	ofxNative::openFile(path);
}


void ofxNative::openFile( string path ){
	NSString * file = [NSString stringWithUTF8String:path.c_str()];
	CGRect rect = CGRectMake(0, 0, ofGetScreenWidth(), ofGetScreenHeight());
	//int sz = [[[NSFileManager defaultManager] attributesOfItemAtPath:file error:nil] fileSize];
	// from: http://developer.apple.com/library/ios/#documentation/FileManagement/Conceptual/DocumentInteraction_TopicsForIOS/Articles/PreviewingandOpeningItems.html#//apple_ref/doc/uid/TP40010410-SW1
	// using static is not the greatest solution,
	// but we do this to retain the interactionController.
	// i have no idea why a raw pointer even does anything, but it definitely does
	// make a difference... in the sense that the static variable works, and a local one doesn't!
	static UIDocumentInteractionController * interactionController;
	interactionController = [UIDocumentInteractionController interactionControllerWithURL: [NSURL fileURLWithPath:file]];
	[interactionController setUTI:@"com.microsoft.waveform-audio"];
	//[interactionController setDelegate:self];
	
	UIView * view = ofxiOSGetGLParentView();
	BOOL isValid = [interactionController presentOpenInMenuFromRect:rect inView:view animated:NO];
	if( isValid ){
		//[interactionController retain];
		//cout << "retaining interaction controller! curret count = " << [interactionController retainCount] << endl;
	}
	else{
		UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"No Apps"
														message:@"You have no apps that support file sharing (for instance, try installing Dropbox)."
													   delegate:nil
											  cancelButtonTitle:@"OK"
											  otherButtonTitles:nil];
		[alert show];
		//[alert release];
	}

	
}

void ofxNative::openUrl( string url_s ){
	NSString * url_ns = [NSString stringWithUTF8String:url_s.c_str()];
	NSURL * url = [NSURL URLWithString:url_ns];
	[[UIApplication sharedApplication] openURL:url];
}


void ofxNative::maximizeWindow( ofAppGLFWWindow & window ){
	std::cerr << "maximize window not implemented" << std::endl;
}


void ofxNative::setMinimumWindowSize( ofAppGLFWWindow & window, int minWidth, int minHeight ){
	std::cerr << "set minimum window size not implemented" << std::endl;
}

void ofxNative::setWindowAlwaysOnTop(ofAppBaseWindow & window, bool onTop){
	std::cerr << "set window always on top not implemented" << std::endl;
}

void ofxNative::setWindowRepresentedFilename(ofAppBaseWindow & window, const string & title, const string & filename ){
	std::cerr << "set window represented filename not implemented" << std::endl;
}


void ofxNative::setMousePositionRelativeToWindow( ofVec2f pos ){
	ofVec2f cgPos =  pos + ofVec2f(ofGetWindowPositionX(),ofGetWindowPositionY());
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
	if(!dir.exists()) dir.create(true);
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
	std::cerr << "system load dialog not implemented" << std::endl;
	ofFileDialogResult results;
	return results;
}

// originally from openFrameworks.
// changed only to include file extensions
ofFileDialogResult ofxNative::systemSaveDialog(string defaultName, string messageName, std::vector<std::string> extensions){
	std::cerr << "system save dialog not implemented" << std::endl;
	ofFileDialogResult results;
	return results;
}

// can prevent/allow system sleep
void ofxNative::setAllowSystemSleep(bool allowSleep){
	[UIApplication sharedApplication].idleTimerDisabled = !allowSleep;
}


#endif
