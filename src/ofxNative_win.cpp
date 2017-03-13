#include "ofxNative.h"

#ifdef _WIN32

#include <winuser.h>
#include <commdlg.h>
#define _WIN32_DCOM
#include <windows.h>

using namespace ofxNative;

void ofxNative::openFile( string filename ){
	int len;
	int slength = (int)filename.length() + 1;
	len = MultiByteToWideChar(CP_ACP, 0, filename.c_str(), slength, 0, 0);
	wchar_t * buf = new wchar_t[len];
	MultiByteToWideChar(CP_ACP, 0, filename.c_str(), slength, buf, len);
	wstring r(buf);
	delete[] buf;
	
	ShellExecute(0, 0, r.c_str(), 0, 0, SW_SHOW);
}


void ofxNative::setCursor(ofxNative::CursorType cursor){
	switch (cursor) {
		case kCursorTypeDefault:
			SetCursor(LoadCursor(nullptr, IDC_ARROW));
			break;
		case kOfxSystemCursorTypeHand:
			SetCursor(LoadCursor(nullptr, IDC_HAND));
			break;
	}
}


#endif