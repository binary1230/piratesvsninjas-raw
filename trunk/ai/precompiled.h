// Precompiled header file.

// ONLY PUT SYSTEM-LEVEL INCLUDES HERE
// do NOT put anything that is in the game engine in here,
// with the exception of rarely-modified stuff

#ifndef PRECOMPILED_H
#define PRECOMPILED_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

// Mac OSX stuff
#ifdef PLATFORM_DARWIN
#include <CoreServices/CoreServices.h>
#include <sys/malloc.h>
#else
#include <malloc.h>
#endif // PLATFORM_DARWIN

// Common to everything
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <png.h>

#ifndef WIN32
#include <sys/wait.h>
#endif

// Unix-y?
#include <ctype.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

// STL stuff
#include <algorithm>
#include <list>
#include <map>
#include <vector>

using namespace std;

#include "StdString.h"

#endif // PRECOMPILED_H
