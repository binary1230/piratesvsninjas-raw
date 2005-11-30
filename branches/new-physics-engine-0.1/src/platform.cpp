// Deals with nasty platform specific stuff.  

#include "platform.h"

#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

//! XXX HACKish, for now.
//!
//! MacOSX bundles (the way we distribute the game) does not allow
//! us to use relative paths because the working directory on MacOSX
//! is not set to the directory where the executable file lives.
//! 
//! What we need to do on Mac is call some Mac-specific code to 
//! find out where the executable lives, and then open our game files
//! relative to that path.
//!
//! On all other platforms, however, we can just use relative paths.
//! This function will attempt to use the MacOSX way first, and if
//! that fails, will just run the relative path way as fallback.
char* get_correct_path(const char* filename) {
	
	const int size=512;
	char *s = (char*)malloc(size);
	
	// path will be set to something like 
	// "/path/to/whatever.app/Resources/" on
	// MacOSX, or else just "" on other platforms.
	const char* path = get_current_working_dir();

	// try the MacOSX path first..
	strncpy(s, path, size-1);
	strncat(s, "Resources/", size-1);
	strncat(s, filename, size-1);

	// if we can't open it that way..
	struct stat sb;
	if (stat(s, &sb) == -1) {
					
		// then return just the filename without any path stuff.
		strncpy(s, filename, size-1);
	}

	return s;
}

#ifdef PLATFORM_DARWIN
#include <CoreServices/CoreServices.h>
#endif 

//! Returns something like '/Applications/Ninjas.app/' on Darwin, else ""
const char* get_current_working_dir() {
#ifdef PLATFORM_DARWIN 
		CFBundleRef mainBundle = CFBundleGetMainBundle();
		CFURLRef url = CFBundleCopyBundleURL(mainBundle);
		CFStringRef cfStr = CFURLCopyPath(url);
		return CFStringGetCStringPtr(cfStr, CFStringGetSystemEncoding());
#else
		return "";
#endif // PLATFORM_DARWIN
}
