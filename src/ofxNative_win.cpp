#ifdef _WIN32

#ifndef UNICODE
#define UNICODE 1
#endif

#include "ofxNative.h"
using namespace ofxNative;

#include <windows.h>
#include <winuser.h>
#include <commdlg.h>
#define _WIN32_DCOM
#include <shlobj.h>
#include <ShObjIdl.h>
#include <shellapi.h>
#include <functional>

// urgh... why not! 
// let's replace the winproc handler...
typedef function<LRESULT(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)> MessageOverride;

static map<HWND, WNDPROC> original_proc;
static map<pair<HWND,UINT>, MessageOverride> msg_overrides;

static LRESULT CALLBACK handle_win_proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
	auto mit = msg_overrides.find(make_pair(hwnd,msg));
	if (mit != msg_overrides.end()) {
		return mit->second(hwnd, msg, wParam, lParam);
	}
	auto oit = original_proc.find(hwnd);
	if (oit != original_proc.end()) {
		return CallWindowProc(oit->second, hwnd, msg, wParam, lParam);
	}

	return 0;
}

static void override_win_proc(HWND hwnd) {
	auto it = original_proc.find(hwnd);
	if (it != original_proc.end()) return;

	WNDPROC original = (WNDPROC)GetWindowLongPtr(hwnd, GWLP_WNDPROC); // use GWL_WNDPROC for 32bit
	SetWindowLongPtr(hwnd, GWLP_WNDPROC, (LONG_PTR)handle_win_proc);
	original_proc[hwnd] = original; 
}

static void override_win_proc(HWND hwnd, UINT msg, MessageOverride func) {
	override_win_proc(hwnd); 
	msg_overrides[make_pair(hwnd, msg)] = func; 
}



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

std::string getExecutablePath() {
	// https://stackoverflow.com/a/33613252/347508
	std::vector<wchar_t> pathBuf;
	DWORD copied = 0;
	do {
		pathBuf.resize(pathBuf.size() + MAX_PATH);
		copied = GetModuleFileNameW(0, &pathBuf.at(0), (DWORD)pathBuf.size());
	} while (copied >= pathBuf.size());

	pathBuf.resize(copied);
	return convertWideToNarrow(pathBuf.data());
}

void ofxNative::showFile(string filename) {
	// argh, something is very wrong here! 
	ofStringReplace(filename, ":/", ":\\"); 
	wstring w = convertNarrowToWide(filename);
	PIDLIST_ABSOLUTE pidl = ILCreateFromPath(w.c_str());
	if (pidl) {
		SHOpenFolderAndSelectItems(pidl, 0, 0, 0);
		ILFree(pidl);
	}
}


void ofxNative::openFile( string filename ){
	wstring r = convertNarrowToWide(filename);
	ShellExecute(0, 0, r.c_str(), 0, 0, SW_SHOW);
}

void ofxNative::openUrl( string url ){
	wstring r = convertNarrowToWide(url);
	ShellExecute(0, L"open", r.c_str(), 0, 0, SW_SHOWNORMAL);
}


void ofxNative::maximizeWindow( ofAppGLFWWindow & window ){
	HWND w32Window = window.getWin32Window();
	ShowWindow(w32Window, SW_MAXIMIZE);
}


void ofxNative::setMinimumWindowSize( ofAppGLFWWindow & window, int minWidth, int minHeight ){
	HWND w32Window = window.getWin32Window();
	override_win_proc(w32Window, WM_GETMINMAXINFO, [minWidth, minHeight](HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
		LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
		lpMMI->ptMinTrackSize.x = minWidth;
		lpMMI->ptMinTrackSize.y = minHeight;
		return 0;
	});
}


void ofxNative::setWindowAlwaysOnTop(ofAppBaseWindow & window, bool onTop){
		HWND w32Window = window.getWin32Window();
		RECT rect;
		GetWindowRect(w32Window, &rect);
		SetWindowPos(w32Window, HWND_TOPMOST, rect.left,rect.top, rect.right-rect.left,rect.bottom-rect.top, SWP_SHOWWINDOW);
}


void ofxNative::setMousePositionRelativeToWindow( ofVec2f pos ){
	// not implemented
	cerr << "ofxNative::setMousePositionRelativeToWindow() not implemented for Windows" << endl;
}

// Thread naming found on SO https://stackoverflow.com/a/23899379/347508

#pragma pack(push,8)
typedef struct tagTHREADNAME_INFO
{
	DWORD dwType; // Must be 0x1000.
	LPCSTR szName; // Pointer to name (in user addr space).
	DWORD dwThreadID; // Thread ID (-1=caller thread).
	DWORD dwFlags; // Reserved for future use, must be zero.
} THREADNAME_INFO;
#pragma pack(pop)


void ofxNative::setThreadName( const string & name){
	DWORD dwThreadID = GetCurrentThreadId();
	THREADNAME_INFO info;
	info.dwType = 0x1000;
	info.szName = name.c_str();
	info.dwThreadID = dwThreadID;
	info.dwFlags = 0;
	
	const DWORD MS_VC_EXCEPTION = 0x406D1388;
	__try{
		RaiseException( MS_VC_EXCEPTION, 0, sizeof(info)/sizeof(ULONG_PTR), (ULONG_PTR*)&info );
	}
	__except(EXCEPTION_EXECUTE_HANDLER){
	}
}

bool ofxNative::canShowConsole() {
	return true; 
}

bool  ofxNative::getConsoleVisible() {
	return (::IsWindowVisible(::GetConsoleWindow()) != FALSE);
}

void ofxNative::setConsoleVisible(bool show) {
	::ShowWindow(::GetConsoleWindow(), show?SW_SHOW:SW_HIDE);
}

std::string ofxNative::getSystemDataFolder() {
	TCHAR szPath[4096];
	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA | CSIDL_FLAG_CREATE, NULL, 0, szPath))) {
		string dirname = convertWideToNarrow(szPath) + "\\" + ofFile(getExecutablePath(),ofFile::Reference).getBaseName();
		ofDirectory(dirname).create();
		return dirname;
	}
	else {
		return ofToDataPath("");
	}
}

std::string ofxNative::getSystemDocumentsFolder() {
	TCHAR szPath[4096];
	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PERSONAL | CSIDL_FLAG_CREATE, NULL, SHGFP_TYPE_CURRENT, szPath))) {
		string dirname = convertWideToNarrow(szPath);
		ofDirectory dir(dirname);
		if(!dir.exists()) dir.create();
		return dirname;
	}
	else {
		return ofToDataPath("");
	}
}

std::string ofxNative::getTempFolder(){
	TCHAR szPath[4096];
	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA, NULL, SHGFP_TYPE_DEFAULT, szPath))) {
		string dirname = convertWideToNarrow(szPath);
		return dirname;
	}
	else {
		return ofToDataPath("");
	}
}


#endif
