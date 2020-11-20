#include <Windows.h>
#include "Game.h"
#include "ViewSprite.h"

static ViewController * pview;

int main()
{

	//hide console
	//ShowWindow(GetConsoleWindow(), SW_HIDE);

	EventManager *evManager = new EventManager();

	KeyboardController keyboard(evManager);
	CPUSpinnerController spinner(evManager);
	ViewController view(evManager);
	GameController game(evManager);

	pview = &view;

	spinner.run();

	//system("PAUSE");

	return 0;


}