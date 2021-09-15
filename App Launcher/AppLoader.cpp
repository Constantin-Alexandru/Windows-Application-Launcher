#include "AppLoader.h"

AppLoader::AppLoader()
{
	init();
	load_apps();
}

bool AppLoader::init()
{
	LPWSTR path = new TCHAR[500];

	HRESULT result = SHGetFolderPath(NULL, CSIDL_MYDOCUMENTS, NULL, SHGFP_TYPE_CURRENT, path);

	if (result != S_OK)
		return false;

	this->folder_path = CW2A(path);

	this->folder_path += "/App Launcher";

	return true;
}

bool AppLoader::load_apps()
{
	if (!fs::exists(this->folder_path))
	{
		fs::create_directory(this->folder_path);
	}

	for (auto const& entry : fs::directory_iterator(this->folder_path))
	{
		this->paths.push_back(std::make_pair<std::string, std::string>(entry.path().stem().string(), entry.path().string()));
	}

	return this->paths.size() > 0;
}

std::vector < std::pair < std::string, std::string> > AppLoader::getPaths()
{
	return this->paths;
}

std::string AppLoader::getPath(std::string name)
{
	for (auto path : this->paths)
	{
		if (path.first == name)
			return path.second;
	}

	return "";
}