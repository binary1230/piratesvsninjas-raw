#ifndef RESOURCE_LOADER_H
#define RESOURCE_LOADER_H

#include <stdio.h>
#include <stdarg.h>
#include <vector>
using namespace std;

#include "gameBase.h"
#include "StdString.h"

//! Resource loader class
//! Resolves resource filenames to absolute paths

class ResourceLoader : public GameBase {
	protected:
		vector<CString> paths;
		
	public:
		int Init(GameState*);
		void Shutdown();
		
		//! Set the search path
		//! Takes at least one path
		void SetSearchPath(char* _path, ...);

		//! Append a new path to the search path
		void AppendToSearchPath(char* _path);

		//! Get the absolute path of this resource in the
		//! search path
		CString GetPathOf(char* filename);

		ResourceLoader();
		~ResourceLoader();
};

#endif // RESOURCE_LOADER_H
