#pragma once

#include "AppLoader.h"

#include <conio.h>
#include <stdio.h>
#include <Windows.h>

#define ENTER '\r'
#define KEY_UP 72
#define KEY_DOWN 80

class App
{
public:

	bool init();

	bool update();

	bool end();

private:

	void setColor(int color);

	void gotoxy(int x, int y);

private:

	HANDLE _handle;

	HWND _hwnd;

	HDC _hdc;

	AppLoader* _loader;

private:

	int selected = 0;

};

