#include "ComponentInput.h"

ComponentInput::ComponentInput()
{
}

ComponentInput::ComponentInput(const ComponentInput&)
{
}

ComponentInput::~ComponentInput()
{
}

int ComponentInput::Initialize()
{
	hConsole = (HANDLE)GetStdHandle(STD_INPUT_HANDLE);
	nLength = 1;
	FlushConsoleInputBuffer(hConsole);
	return 1;
}

KEY_EVENT_RECORD ComponentInput::GetKeypress()
{
	DWORD eventsRead;

	GetNumberOfConsoleInputEvents(hConsole, &eventsToRead);
	//Check to see if a key has been pressed
	if (eventsToRead >= 1)
	{
		//get keyboard input
		ReadConsoleInput(hConsole, &inputRecord, nLength, &eventsRead);
		if (inputRecord.Event.KeyEvent.bKeyDown == true)
			prevKeyEventRecord = inputRecord.Event.KeyEvent;
	}
	//Sleep just to stall the keyspamm
	Sleep(75);
	return inputRecord.Event.KeyEvent;
}
