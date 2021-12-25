#pragma once

#include <string>
#include <vector>
#include <stdio.h>
#include <utility>
#include <shlobj.h>
#include <atlstr.h>
#include <filesystem>

namespace fs = std::filesystem;

class AppLoader
{
public:

	AppLoader();

	bool init();
	bool load_apps();

	std::vector < std::pair < std::string, std::string> > getPaths();
	std::string getPath(std::string name);

private:
	
	std::vector < std::pair < std::string, std::string> > paths;
	std::string folder_path;
};

