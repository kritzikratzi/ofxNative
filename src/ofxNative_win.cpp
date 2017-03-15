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


void ofxNative::setCursor(ofxNative::CursorType cursor){
	switch (cursor) {
	case kCursorTypeDefault:
			SetCursor(LoadCursor(nullptr, IDC_ARROW));
			break;
	case kCursorTypeHand:
			SetCursor(LoadCursor(nullptr, IDC_HAND));
			break;
	}
}


#endif