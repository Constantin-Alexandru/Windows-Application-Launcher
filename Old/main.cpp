#include "App.h"

int main()
{
	App* app = new App();

	if (!app->init())
		return 0;
	if (!app->update())
		return 0;
	if (!app->end())
		return 0;

	return 0;
}