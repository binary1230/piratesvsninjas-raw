#include "resourceLoader.h"

void ResourceLoader::AppendToSearchPath(char* path) {
	paths.push_back(path);
}

void ResourceLoader::SetSearchPath(char* path, ...) {
	
}

CString ResourceLoader::GetPathOf(char* filename) {
	
}

ResourceLoader::ResourceLoader() {
}

ResourceLoader::~ResourceLoader() {
}
