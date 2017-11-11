# ofxNative


Native functions for openFrameworks


## Overview


	✓  = Implemented
	✓? = Implemented, could/should work, but untested
	𐄂  = Missing


Method|Description|Windows|OSX
-------|------------------|---|---
`showFile(string path)`|Reveals a file in explorer/finder|✓|✓
`openFile(string path)`|Opens a file with the default application|✓|✓
`setMinimumWindowSize(GLFWWindow & window, int w, int h)`|Sets the minimum size of a window|𐄂|✓
`setWindowAlwaysOnTop(GLFWWindow & window, bool onTop)`|Toggles "float on top" for a window|✓|✓
`maximizeWindow(GLFWWindow & window)`|Maximizes the window|✓|✓
`setThreadName(const string & name)`|Sets the name of the current thread. Note:Renaming _other_ threads would be possible in Windows, but not in OSX|✓|✓


## Contributing 

When adding methods/implementations make sure to update this documentation as well. 

## License

MIT License