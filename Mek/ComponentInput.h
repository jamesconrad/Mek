#ifndef ComponentInput_H 
#define ComponentInput_H


//ComponentInput to get keyboard input. 
#include <iostream>
#include <Windows.h>
#include <Xinput.h>

class ComponentInput
{
public:
	ComponentInput() : deadzoneX(0.05f), deadzoneY(0.02f) {}
	ComponentInput(float dzX, float dzY) : deadzoneX(dzX), deadzoneY(dzY) {}

	float leftStickX;
	float leftStickY;
	float rightStickX;
	float rightStickY;
	float leftTrigger;
	float rightTrigger;

	int  GetPort();
	XINPUT_GAMEPAD *GetState();
	bool CheckConnection();
	bool Refresh();
	bool IsPressed(WORD);

private:
	int cId;
	XINPUT_STATE state;

	float deadzoneX;
	float deadzoneY;
};


#endif
