#include "ofxNative.h"

#ifdef _WIN32

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

#endif