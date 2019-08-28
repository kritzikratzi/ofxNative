# ofxNative


Native functions for openFrameworks


## Overview


	✓  = Implemented
	✓? = Implemented, could/should work, but untested
	✗  = Missing (implemented, but does nothing... calling it won't hurt)



Method|Description|Windows|OSX|Linux
-------|------------------|---|---|---
`showFile(string path)`|Reveals a file in explorer/finder|✓|✓|✓<sup>1</sup>
`openFile(string path)`|Opens a file with the default application|✓|✓|✓
`setMinimumWindowSize(GLFWWindow & window, int w, int h)`|Sets the minimum size of a window|✗|✓|✗
`setWindowAlwaysOnTop(GLFWWindow & window, bool onTop)`|Toggles "float on top" for a window|✓|✓|✗
`maximizeWindow(GLFWWindow & window)`|Maximizes the window|✓|✓|✗
`setThreadName(const string & name)`|Sets the name of the current thread. Note:Renaming _other_ threads would be possible in Windows, but not in OSX|✓|✓|✗
`canShowConsole()`|Checks if there is a attached system console that can be shown/hidden|✓|✗|✗
`getConsoleVisible()`|Returns true if the console is visible|✓|✗|✗
`setConsoleVisible(bool show)`|Shows/Hides the windows system console. This requires the linker flag `subsystem:console`|✓|✗|✗
`getSystemDataFolder()`|Returns a location in a global folder (Application-Support on OSX, %APPDATA% on windows)|✓|✓|✓<sup>2</sup>

1. same Behaviour as `openFile()`
2. returns binary folder

## Contributing

When adding methods/implementations make sure to update this documentation as well.

## License

MIT License
