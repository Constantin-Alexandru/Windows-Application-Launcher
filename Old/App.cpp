#include "App.h"

bool App::init()
{
	this->_loader = new AppLoader();

	this->_hwnd = GetActiveWindow();

	this->_hdc = GetDC(this->_hwnd);

	this->_handle = GetStdHandle(STD_OUTPUT_HANDLE);

	if (!SetConsoleTitle(L"App Launcher"))
	{
		printf("Error: Failed to set console title");
		return false;
	}
	COORD size;

	size.X = 1080;
	size.Y = 720;

	return true;
}

bool App::update()
{
	while (true)
	{
		
		int startX = 10, startY = 10;

		auto paths = this->_loader->getPaths();
		
		for (int i = 0; i < paths.size(); i++)
		{
			App::gotoxy(startX, startY + i);

			if (i == this->selected)
			{
				setColor(240); 
			}
			else {
				setColor(15);
			}


			printf("%s\n", paths[i].first.c_str());
		}
	
		char c;

		switch ((c = _getch()))
		{
			case KEY_UP:
				selected--;
				selected = selected < 0 ? paths.size() - 1 : selected;
				break;	
			case KEY_DOWN:
				selected++;
				selected = selected >= paths.size() ? 0 : selected;
				break;
			case ENTER:
				ShellExecuteA(NULL, NULL, paths[selected].second.c_str(), NULL, NULL, 0);
				break;

			default:
				break;
		}

	}

	return true;
}

bool App::end()
{
	delete this->_loader;

	return true;
}

void App::setColor(int color)
{
	SetConsoleTextAttribute(this->_handle, color);
}

void App::gotoxy(int x, int y)
{
	COORD c;
	c.X = x;
	c.Y = y;
	SetConsoleCursorPosition(this->_handle, c);
}