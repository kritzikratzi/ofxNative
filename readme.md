# ofxNative


Native functions for openFrameworks


## Overview


	✓  = Implemented
	※  = Uses a workaround, because it doesn't exist in that form for this operating system
	✗  = Missing (implementation empty... calling it won't hurt, but does nothing)



Method|Description|Windows|macos|Linux|iOS
-------|------------------|---|---|---
`showFile(string path)`|Reveals a file in explorer/finder|✓|✓|✓|※
`openFile(string path)`|Opens a file with the default application|✓|✓|✓|untested
`openUrl(string path)`|Opens a url with the default browser|✓|✓|✓|untested
`setMinimumWindowSize(GLFWWindow & window, int w, int h)`|Sets the minimum size of a window|✓|✓|✗|✗
`setWindowAlwaysOnTop(GLFWWindow & window, bool onTop)`|Toggles "float on top" for a window|✓|✓|✗|✗
`setWindowRepresentedFilename(GLFWWindow & window, string title, string filename)`|Shows title and/or filename|✓|※|※|✗
`maximizeWindow(GLFWWindow & window)`|Maximizes the window|✓|✓|✗|✗
`setThreadName(const string & name)`|Sets the name of the current thread. Note:Renaming _other_ threads would be possible in Windows, but not in OSX|✓|✓|✗|✓
`canShowConsole()`|Checks if there is a attached system console that can be shown/hidden|✓|✗|✗|✗
`getConsoleVisible()`|Returns true if the console is visible|✓|✗|✗|✗
`setConsoleVisible(bool show)`|Shows/Hides the windows system console. This requires the linker flag `subsystem:console`|✓|✗|✗|✗
`getSystemDataFolder()`|Returns a location in a global folder (Application-Support on OSX, `%APPDATA%` on windows), `~/.local/share` on linux|✓|✓|✓|✓
`getSystemDocumentsFolder()`|Returns the location of the user's documents folder|untested|✓|missing|untested
`getTempFolder()`|Path to a system wide temp folder|untested|✓|untested|untested
`systemLoadDialog(std::string windowTitle, bool bFolderSelection, std::string defaultPath, std::vector<std::string> extensions)`|Shows the system's "load file" popup|✓|✓|✗|✗
`setAllowSystemSleep(bool)`|Prevent screensaver/system sleep from happening|✗|✓|✗|✓

## Contributing

When adding methods/implementations make sure to update this documentation as well.

## License

MIT License
