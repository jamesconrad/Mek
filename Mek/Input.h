#ifndef INPUTCLASS_H 
#define INPUTCLASS_H


//InputClass to get keyboard input. 
#include <Windows.h>


class InputClass
{
private:
	HANDLE hConsole, hThread;
 	DWORD nLength, eventsToRead;
 	DWORD * eventsRead;
	INPUT_RECORD inputRecord;
 	KEY_EVENT_RECORD prevKeyEventRecord;
		

public:
	InputClass();
 	InputClass(const InputClass&);
 	~InputClass();
				

 	int Initialize();
 	KEY_EVENT_RECORD GetKeypress();
};


#endif
