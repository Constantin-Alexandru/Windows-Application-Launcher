#include "main.h"


Data getInitialData();
void updateData(Data data);

std::string getApplicationsPath(std::string directoryName);

std::vector<std::string> getApplications(std::string path, bool settings);

int getLineCount();

void printApplications(std::vector<std::string> applications, int selectedApplication);

Actions getAction();

void HandleApplication(std::string application);

void HandleSetting(std::string setting, std::string path, bool& settings, int& selectedApplication, Data& data);

int main()
{


	Data data = getInitialData();

	std::string path = getApplicationsPath(data.directoryName);
	
	if (path == "")
	{
		exit(-1);
	}

	int selectedApplication = 0;
	bool settings = false;

	while (true)
	{
		std::ios_base::sync_with_stdio(0);
		system("CLS");

		std::vector<std::string> applications = getApplications(path, settings);
		
		printApplications(applications, selectedApplication);

		Actions action = getAction();

		static_assert(Actions::Count == (Actions)4, "Unhandled Actions");

		switch (action)
		{
		case Actions::MoveUp:
			if (selectedApplication == 0)
			{
				selectedApplication = applications.size();
			}

			selectedApplication--;
			break;
		case Actions::MoveDown:
			selectedApplication++;
			if (selectedApplication == applications.size())
			{
				selectedApplication = 0;
			}
			break;
		case Actions::Select:
			if(!settings && selectedApplication == applications.size() - 1)
			{
				settings = true;
				selectedApplication = 0;
			}
			else if(!settings) {
				HandleApplication( path + "\\" + applications[selectedApplication]);
			}
			else {
				HandleSetting(applications[selectedApplication], path + "\\", settings, selectedApplication, data);
				path = getApplicationsPath(data.directoryName);
			}
			break;
		case Actions::Exit:
			if (settings) {
				selectedApplication = 0;
				settings = false;
			}
			else exit(0);
			break;
		}

	}
}

Data getInitialData()
{
	Data data;

	std::ifstream in("data.txt");

	std::getline(in, data.directoryName);

	in.close();

	return data;
}

void updateData(Data data)
{
	std::ofstream out("data.txt");

	out << data.directoryName;

	out.close();
}

std::string getApplicationsPath(std::string directoryName)
{
	TCHAR t_path[MAX_PATH];
	HRESULT result = SHGetFolderPath(NULL, CSIDL_PERSONAL, NULL, SHGFP_TYPE_CURRENT, t_path);

	if (result != S_OK)
	{
		std::cout << "ERROR: COULD NOT FIND DOCUMENTS PATH \n";

		return "";
	}

	std::wstring w_path(&t_path[0]);
	std::string path(w_path.begin(), w_path.end());

	path += "\\";
	path += directoryName;

	return path;
}

int getLineCount()
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	int rows;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;

	return rows;
}

int getColumnCount()
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	int columns;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
	columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;

	return columns;
}

void printApplications(std::vector<std::string> applications, int selectedApplication) {
	
	int titleColumns = getColumnCount() / 2 - 10;

	std::string titleSpaces(titleColumns, ' ');

	int newlines = 3;

	std::string newLines(newlines, '\n');
	
	std::cout << titleSpaces << "Application Launcher" << newLines;

	int itemColumns = getColumnCount() / 6;

	std::string itemSpaces(itemColumns, ' ');

	int rows = getLineCount() - newlines - 1;

	int start = selectedApplication >= rows ? selectedApplication - rows + 1 : 0;

	for (int i = 0; i < rows && i < applications.size(); i++)
	{
		std::cout << itemSpaces;
		if (selectedApplication == start + i)
			std::cout << " > ";
		std::cout << applications[start + i] << '\n';
	}

}


namespace fs = std::filesystem;

std::vector<std::string> getApplications(std::string path, bool settings)
{
	std::vector<std::string> applications;

	if (!settings)
	{
		if (fs::exists(path))
		{
			for (auto const& entry : fs::directory_iterator(path))
			{
				applications.push_back(entry.path().stem().string());
			}
		}
		else {
			std::cout << "DIRECTORY " << path << " DOES NOT EXIST. CREATE IT FIRST!\n";
		}

		applications.push_back("Settings");
	}
	else {
		applications.push_back("Open Directory");
		applications.push_back("Change Directory");
		applications.push_back("Exit Settings");
	}

	return applications;
}

Actions getAction() {
	char c;

	switch ((c = _getch()))
	{
	case KEY_UP:
		return Actions::MoveUp;
	case KEY_DOWN:
		return Actions::MoveDown;
	case ENTER:
		return Actions::Select;
	case ESCAPE:
		return Actions::Exit;
	}
}

void HandleApplication(std::string application)
{
	ShellExecuteA(NULL, "open", application.c_str(), NULL, NULL, 0);
}

void HandleSetting(std::string setting, std::string path, bool &settings, int &selectedApplication, Data &data)
{
	if (setting == "Open Directory")
	{
		std::string cmd = "explorer " + path;

		system(cmd.c_str());

	}
	else if (setting == "Change Directory")
	{
		system("CLS");

		std::cout << "New Directory : ";

		std::string newDirectory;

		std::getline(std::cin, newDirectory);

		data.directoryName = newDirectory;

		updateData(data);

	}
	else if (setting == "Exit Settings")
	{
		selectedApplication = 0;
		settings = false;
	}
}