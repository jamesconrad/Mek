//Brent Cowan 2014

#include "ConsoleMagic.h"

ConsoleMagic::ConsoleMagic()
{
	charInfo = NULL;
}

ConsoleMagic::~ConsoleMagic()
{
	//Delete the dynamically alocated memory
	if(charInfo!=NULL)delete [] charInfo;
}
void ConsoleMagic::Init(unsigned short width, unsigned short height)
{
	//initialize handles
	wHnd = GetStdHandle(STD_OUTPUT_HANDLE);
	rHnd = GetStdHandle(STD_INPUT_HANDLE);

    bufferSize.X = width; 
	bufferSize.Y = height; 

    windowSize.Top = 0; windowSize.Left = 0; 
    windowSize.Bottom = height - 1; 
    windowSize.Right = width - 1;

	characterPosition.X = 0; characterPosition.Y = 0;

    //Get handle of the standard output  
    if (wHnd == NULL) {return;} 
     
    //Set screen buffer size to that specified in coord 
    if(!SetConsoleScreenBufferSize(wHnd, bufferSize)) {return;} 

    //Set the window size to that specified in Rect 
    if(!SetConsoleWindowInfo(wHnd, TRUE, &windowSize)) {return;}

	//Call the window "Console Magic"
	SetTitle("Console Magic");

	//Allocate Memory for the character buffer
	charInfo = new CHAR_INFO[width*height];

	//Clear the screen by filling it with spaces with a black background
	for(unsigned short x = 0; x < width; x++)
	{
		for(unsigned short y = 0; y < height; y++)
		{
			charInfo[x + y*width].Char.AsciiChar = ' ';
			charInfo[x + y*width].Attributes = 0;
		}
	}
}

void ConsoleMagic::SetTitle(char* title)
{
	//Convert the character array to wide char
	string wideString = title;
	//for(int i = 0; title[i]!=NULL; i++)
	//{
	//	wideString = wideString + wchar_t(title[i]);
	//}

	//Set the title
	SetConsoleTitle(wideString.c_str());
}

void ConsoleMagic::SetCharacter(unsigned short x, unsigned short y, char character)
{
	//Make sure the character is on the screen
	if(x>=bufferSize.X)return;
	if(y>=bufferSize.Y)return;

	charInfo[x + y*bufferSize.X].Char.AsciiChar = character;
}

char ConsoleMagic::GetCharacter(unsigned short x, unsigned short y)
{
	//Make sure the character is on the screen
	if(x>=bufferSize.X)return 0;
	if(y>=bufferSize.Y)return 0;

	return charInfo[x + y*bufferSize.X].Char.AsciiChar;
}

void ConsoleMagic::SetForegroundColor(unsigned short x, unsigned short y, unsigned char color)
{
	//Make sure the character is on the screen
	if(x>=bufferSize.X)return;
	if(y>=bufferSize.Y)return;

	tmp = (unsigned char)charInfo[x + y*bufferSize.X].Attributes;
	tmp = tmp/16;
	tmp = tmp*16 + (color%16);

	charInfo[x + y*bufferSize.X].Attributes = tmp;
}

void ConsoleMagic::SetBackgroundColor(unsigned short x, unsigned short y, unsigned char color)
{
	//Make sure the character is on the screen
	if(x>=bufferSize.X)return;
	if(y>=bufferSize.Y)return;

	tmp = (unsigned char)charInfo[x + y*bufferSize.X].Attributes;
	tmp = tmp&15;
	tmp = tmp + (color%16)*16;

	charInfo[x + y*bufferSize.X].Attributes = tmp;
}

void ConsoleMagic::Clear(char character, unsigned char background, unsigned char foreground)
{
	//Set every character to the character supplied
	tmp = (background%16)*16+(foreground%16);
	for(unsigned short x = 0; x < bufferSize.X; x++)
	{
		for(unsigned short y = 0; y < bufferSize.Y; y++)
		{
			charInfo[x + y*bufferSize.X].Char.AsciiChar = character;
			charInfo[x + y*bufferSize.X].Attributes = tmp;
		}
	}
}

void ConsoleMagic::Update()
{
	//copy the buffer to the console
	WriteConsoleOutputA(wHnd, charInfo, bufferSize, characterPosition, &windowSize);
}

void ConsoleMagic::DrawFrame(unsigned short x, unsigned short y, 
	unsigned short width, unsigned short height, unsigned char background,
	unsigned char foreground, unsigned char style)
{
	//Make sur the rectangle is on screen
	if(width<2 || height<2)return;
	if(x+width>bufferSize.X)return;
	if(y+height>bufferSize.Y)return;

	unsigned char topLeft, topRight, bottomLeft, bottomRight, horizontal, vertical;

	//Set the style
	style = style%6;
	if(style==5)
	{
		topLeft     = 177;
		topRight    = 177;
		bottomLeft  = 177;
		bottomRight = 177;
		horizontal  = 177;
		vertical    = 177;
	}
	else if(style==4)
	{
		topLeft     = 254;
		topRight    = 254;
		bottomLeft  = 254;
		bottomRight = 254;
		horizontal  = 254;
		vertical    = 254;
	}
	else if(style==3)
	{
		topLeft     = 4;
		topRight    = 4;
		bottomLeft  = 4;
		bottomRight = 4;
		horizontal  = 4;
		vertical    = 4;
	}
	else if(style==2)
	{
		topLeft     = 3;
		topRight    = 3;
		bottomLeft  = 3;
		bottomRight = 3;
		horizontal  = 3;
		vertical    = 3;
	}
	else if(style==1)
	{
		topLeft     = 201;
		topRight    = 187;
		bottomLeft  = 200;
		bottomRight = 188;
		horizontal  = 205;
		vertical    = 186;
	}
	else
	{
		topLeft     = 218;
		topRight    = 191;
		bottomLeft  = 192;
		bottomRight = 217;
		horizontal  = 196;
		vertical    = 179;
	}

	tmp = (background%16)*16+(foreground%16);

	//Draw corners
	charInfo[x + y*bufferSize.X].Char.AsciiChar = topLeft;
	charInfo[x + y*bufferSize.X].Attributes = tmp;

	charInfo[x+width-1 + y*bufferSize.X].Char.AsciiChar = topRight;
	charInfo[x+width-1 + y*bufferSize.X].Attributes = tmp;

	charInfo[x + (y+height-1)*bufferSize.X].Char.AsciiChar = bottomLeft;
	charInfo[x + (y+height-1)*bufferSize.X].Attributes = tmp;

	charInfo[x+width-1 + (y+height-1)*bufferSize.X].Char.AsciiChar = bottomRight;
	charInfo[x+width-1 + (y+height-1)*bufferSize.X].Attributes = tmp;

	//Draw the top and bottom
	if(width>2)
	{
		for(int i = 1; i<width-1; i++)
		{
			charInfo[x+i + y*bufferSize.X].Char.AsciiChar = horizontal;
			charInfo[x+i + y*bufferSize.X].Attributes = tmp;

			charInfo[x+i + (y+height-1)*bufferSize.X].Char.AsciiChar = horizontal;
			charInfo[x+i + (y+height-1)*bufferSize.X].Attributes = tmp;
		}
	}

	//Draw the sides
	if(height>2)
	{
		for(int i = 1; i<height-1; i++)
		{
			charInfo[x + (y+i)*bufferSize.X].Char.AsciiChar = vertical;
			charInfo[x + (y+i)*bufferSize.X].Attributes = tmp;

			charInfo[x+width-1 + (y+i)*bufferSize.X].Char.AsciiChar = vertical;
			charInfo[x+width-1 + (y+i)*bufferSize.X].Attributes = tmp;
		}
	}
}