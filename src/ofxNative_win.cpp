#include "ofxNative.h"

#ifdef _WIN32

#include <winuser.h>
#include <commdlg.h>
#define _WIN32_DCOM
#include <windows.h>
#include <shlobj.h>

using namespace ofxNative;

// utf-16 to utf-8
static std::string convertWideToNarrow(const wchar_t *s, char dfault = '?',
	const std::locale& loc = std::locale())
{
	std::ostringstream stm;

	while (*s != L'\0') {
		stm << std::use_facet< std::ctype<wchar_t> >(loc).narrow(*s++, dfault);
	}
	return stm.str();
}


// utf-8 to utf-16
static std::wstring convertNarrowToWide(const std::string& as) {
	// deal with trivial case of empty string
	if (as.empty())    return std::wstring();

	// determine required length of new string
	size_t reqLength = ::MultiByteToWideChar(CP_UTF8, 0, as.c_str(), (int)as.length(), 0, 0);

	// construct new string of required length
	std::wstring ret(reqLength, L'\0');

	// convert old string to new string
	::MultiByteToWideChar(CP_UTF8, 0, as.c_str(), (int)as.length(), &ret[0], (int)ret.length());

	// return new string ( compiler should optimize this away )
	return ret;
}


void ofxNative::showFile(string filename) {
	wstring w = convertNarrowToWide(filename); 
	ITEMIDLIST *pidl = ILCreateFromPath(w.c_str());
	if (pidl) {
		SHOpenFolderAndSelectItems(pidl, 0, 0, 0);
		ILFree(pidl);
	}
}


void ofxNative::openFile( string filename ){
	wstring r = convertNarrowToWide(filename); 
	ShellExecute(0, 0, r.c_str(), 0, 0, SW_SHOW);
}


void ofxNative::maximizeWindow( ofAppGLFWWindow & window ){
	HWND w32Window = window.getWin32Window();
	ShowWindow(w32Window, SW_MAXIMIZE);
}


void ofxNative::setMinimumWindowSize( ofAppGLFWWindow & window, int minWidth, int minHeight ){
	// not implemented
	cerr << "ofxNative::setMinimumSize() not implemented for Windows" << endl;
}


void ofxNative::setMousePositionRelativeToWindow( ofVec2f pos ){
	// not implemented
	cerr << "ofxNative::setMousePositionRelativeToWindow() not implemented for Windows" << endl;
}


#endif