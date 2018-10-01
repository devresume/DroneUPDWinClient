#include <map>
#include <vector>
#include <Windows.h>

#define PITCHFORWARD VK_UP
#define PITCHBACKWARD VK_DOWN
#define ROLLLEFT VK_LEFT
#define ROLLRIGHT VK_RIGHT

//#define PITCHFORWARD 0x26
//#define PITCHBACKWARD 0x28
//#define ROLLLEFT 0x25
//#define ROLLRIGHT 0x27
#define YAWLEFT 'A'
#define YAwRIGHT 'D'
#define THROTTLEUP 'W'
#define THROTTLEDOWN 'S'
#define EXITBUTTON 0x08

enum controls : int
{
	PUP = 1,
	PBACK = 2,
	RLEFT = 4,
	RRIGHT = 8,
	YLEFT = 16,
	YRIGHT = 32,
	TUP = 64,
	TDOWN = 128,
	EXIT = 256,
};

std::map<int, int> getCommandMap()
{
	static std::map<int, int> commandMap;
	commandMap[PITCHFORWARD] = PUP;
	commandMap[PITCHBACKWARD] = PBACK;
	commandMap[ROLLLEFT] = RLEFT;
	commandMap[ROLLRIGHT] = RRIGHT;
	commandMap[YAWLEFT] = YLEFT;
	commandMap[YAwRIGHT] = YRIGHT;
	commandMap[THROTTLEUP] = TUP;
	commandMap[THROTTLEDOWN] = TDOWN;
	commandMap[EXITBUTTON] = EXIT;
	return commandMap;
}

int getCommandKeyPress(bool& isExit)
{
	int command = 0;
	static std::map<int, int> commandMap = getCommandMap();
	
	std::map<int, int>::const_iterator iter;

	for (iter = commandMap.begin(); iter != commandMap.end(); ++iter)
	{
		if (GetAsyncKeyState(iter->first))
		{
			if (iter->second == EXIT)
			{
				isExit = true;
			}
			command += iter->second;
		}
	}
	return command;
}

std::vector<int> getCommandTypes(int command)
{
	static std::map<int, int> commandMap = getCommandMap();

	std::map<int, int>::const_iterator iter;
	std::vector<int> commandList;

	for (iter = commandMap.begin(); iter != commandMap.end(); ++iter)
	{
		if (iter->second & command)
		{
			commandList.push_back(iter->second);
		}
	}
	return commandList;
}
#pragma once
