#ifndef ComponentInput_H 
#define ComponentInput_H


//ComponentInput to get keyboard input. 
#include <Windows.h>


class ComponentInput
{
private:
	HANDLE hConsole, hThread;
 	DWORD nLength, eventsToRead;
 	DWORD * eventsRead;
	INPUT_RECORD inputRecord;
 	KEY_EVENT_RECORD prevKeyEventRecord;
		

public:
	ComponentInput();
 	ComponentInput(const ComponentInput&);
 	~ComponentInput();
				

 	int Initialize();
 	KEY_EVENT_RECORD GetKeypress();
};


#endif
