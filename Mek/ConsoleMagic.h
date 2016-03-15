#pragma once
//Brent Cowan 2014

/*
This class allows us to place characters on the console screen and set the 
foreground colour (symbol colour) and background colour of each character. 
Once all of the characters are placed, the Update() function is called to 
update the console. The screen is updated quickly and without the tearing 
caused by printing one line at a time using cout.

The ASCII symbols available and their codes can be found here:
http://www.ascii-codes.com/

The 16 available colours and their codes can be found here:
http://ss64.com/nt/color.html

There are 6 rectangle frame styles (0-5).
*/

#include <iostream>
#include <windows.h>
#include <string>
using namespace std;

class ConsoleMagic
{
private:
	HANDLE wHnd; //Write (output) handle
	HANDLE rHnd; //Read (input handle
	SMALL_RECT windowSize;
	COORD bufferSize;
	COORD characterPosition;
	CHAR_INFO* charInfo;
	unsigned char tmp;

public:
	ConsoleMagic();
	~ConsoleMagic();

	//Always initialize the console before reading or writing to it
	void Init(unsigned short width, unsigned short height);

	//Change the title of the window
	void SetTitle(char* title);

	//Place a character on the screen
	void SetCharacter(unsigned short x, unsigned short y, char character);

	//Set the colour of the character at the given location
	void SetForegroundColor(unsigned short x, unsigned short y, unsigned char color);

	//Set the background colour of the character at the given location
	void SetBackgroundColor(unsigned short x, unsigned short y, unsigned char color);

	//Clear the screen by setting every character and colour
	void Clear(char character, unsigned char background, unsigned char foreground);

	//Draw a rectangular frame
	void DrawFrame(unsigned short x, unsigned short y, 
		unsigned short width, unsigned short height, unsigned char background,
		unsigned char foreground, unsigned char style);

	//Get the character at a specific location
	char GetCharacter(unsigned short x, unsigned short y);

	//Update the console
	void Update();
};