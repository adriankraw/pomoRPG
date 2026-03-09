#pragma once

#include <algorithm>
#include <cassert>
#include <cmath>
#include <codecvt>
#include <cstddef>
#include <functional>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

#include "render.cpp"
#include "Character.cpp"
#include "Eventable/ItemDrop.h"
#include "Commands.cpp"
#include "logger.cpp"

class Printer{
public:
	Printer();
	Printer(Printer &&) = default;
	Printer(const Printer &) = default;
	Printer &operator=(Printer &&) = default;
	Printer &operator=(const Printer &) = default;
	~Printer();

	enum windowType{
		empty,
		bigclock,
		charsettings,
		input,
		stopwatches,
		eventlist,
		fight,
		skillAnimation,
		help,
	};

	struct window{
		bool isDrawn = false;
		bool border = false;	
		int positionX = 0;
		int positionY = 0;
		int width = 0;
		int height = 0;
		int animationframe = 0;
		bool animatin_isRunning = false;
		char* text[1024];
	};

	void Header();
	void Timer(Time &currentTime);
	void CharacterStats(Character& character);
	void Flush();
	void Bar(std::string,int, int, int);
	void OpenFightScreen(Character& character, std::shared_ptr<Monster>& monster);
	void EventsList(std::vector<std::tuple<Character::CharEvent, std::function<void*()>>>& events, std::shared_ptr<Monster> currentMonster);
	void PrintSkillAnimation(Char::AnimationType type);
	void Help();
	void PrintScreen();
	void CalcLineBreak();
	void SetSize(int width, int height){
		screenWidth = width;
		screenHeight = height;
		CalcLineBreak();
	}
	void PassStringToBuffer(int index, std::string string);
	void CreateBox(window& _window);

	void ToggleWindow(windowType type);
	std::map<windowType, window> windows = {
		{windowType::bigclock,		{ .isDrawn = true, .border = true, 
			.positionX = 0, .positionY = 1, .width = 50, .height = 6}
		},
		{windowType::charsettings,	{ .isDrawn = true, .border = true, 
			.positionX = 70,.positionY = 1, .width = 40, .height = 15}
		},
		{windowType::input,		{ .isDrawn = true, .border = false, 
			.positionX = 0, .positionY = 0, .width = 0, .height = 0}
		},
		{windowType::stopwatches,	{ .isDrawn = true, .border = true, 
			.positionX = 5, .positionY = 17, .width = 0, .height = 5}
		},
		{windowType::eventlist,		{ .isDrawn = true, .border = true, 
			.positionX = 0, .positionY = 9, .width = 0, .height = 7}
		},
		{windowType::fight,		{ .isDrawn = true, .border = true, 
			.positionX = 0, .positionY = 36, .width = 0, .height = 0}
		},
		{windowType::skillAnimation,	{ .isDrawn = true, .border = false, 
			.positionX = 0, .positionY = 50, .width = 0, .height = 0}
		},
		{windowType::help,		{ .isDrawn = true, .border = false, 
			.positionX = 0, .positionY = 49, .width = 0, .height = 0}
		},
	};
private:
	render ren;
	std::vector<std::string> screenbuffer = {50, "\s"};
	std::string linebreak = "";
	int screenWidth = 0;
	int screenHeight = 0;

	logger charlogger{"printer.log"};
};

Printer::Printer() {
	CalcLineBreak();
	std::cout << "\033[2J" << std::flush;
	screenbuffer.reserve(50);
	for(auto iter = screenbuffer.begin(); iter != screenbuffer.end(); ++iter)
	{
		iter->clear();
		for(size_t spaces = 0; spaces < screenWidth; ++spaces)
		{
			iter->push_back('\s');
		}
	}
	std::string x = "█";
	for(auto iter = 0; iter < x.size(); ++iter)
	{
		charLogger.logInfo(std::to_string(x.at(iter)));
	}
}

Printer::~Printer() {
	screenbuffer.clear();
	std::cout << std::flush;
}

void Printer::CalcLineBreak(){
	linebreak = "";
	if(screenHeight> 5)
	screenbuffer.resize(screenHeight-2,"\s");

	if(screenWidth > 5)
	for(size_t i = 0; i < (screenWidth-5); ++i)
	{
		linebreak.append("-");	
	}
}

void Printer::Header() {
	int lastLine = 0;
	PassStringToBuffer(lastLine, "PomoRPG:"+std::to_string(screenWidth)+":"+std::to_string(screenHeight));
}
void Printer::Timer(Time &currentTime){
	int lastLine = windows[bigclock].positionY;
	ren.renderTime(currentTime);
	std::string offsetX = "";
	if(windows[bigclock].positionX > 0) {
		offsetX.resize(windows[bigclock].positionX, ' ');
	}

	PassStringToBuffer(lastLine, offsetX+"---BigClock");
	size_t resultSize = ren.result.size();
	for(size_t i = 0; i < resultSize; ++i)
	{
		PassStringToBuffer(++lastLine, offsetX+(ren.result[i]));
	}
	//PassStringToBuffer(++lastLine, offsetX+"\033[0m");
	windows[bigclock].height = lastLine+1-windows[bigclock].positionY;
	CreateBox(windows[bigclock]);
}
void Printer::CharacterStats(Character& character){
	int lastLine = windows[charsettings].positionY;
	std::string offsetX = "";
	if(windows[charsettings].positionX > 0) {
		offsetX.resize(windows[charsettings].positionX, ' ');
	}
	PassStringToBuffer(lastLine, offsetX+"---CharStats");
	PassStringToBuffer(++lastLine, offsetX+"RPG:");

	PassStringToBuffer(++lastLine, offsetX+"Name      "+character.Name());
	PassStringToBuffer(++lastLine, offsetX+"LIFE      "+std::to_string(character.Life()));
	PassStringToBuffer(++lastLine, offsetX+"ATK       "+std::to_string(character.Atk()));
	PassStringToBuffer(++lastLine, offsetX+"Def       "+std::to_string(character.Def()));
	PassStringToBuffer(++lastLine, offsetX+"LVL       "+std::to_string(character.Lvl()));
	PassStringToBuffer(++lastLine, offsetX+"Exp       "+std::to_string(character.Exp())+"/"+std::to_string(character.GetNextLevelExp()));
	PassStringToBuffer(++lastLine, offsetX+"ExpMul    "+std::to_string(character.Expmultiplier()));
	int skillcount = 0;	
	for(auto& skill: character.skillList)
	{
		PassStringToBuffer(++lastLine, offsetX+"Skill:    "+ skill->name +": "+ std::to_string(skill->expToLevel)+" cost: "+std::to_string(skill->cost));
		skillcount++;
	}

	windows[charsettings].height = lastLine+2-windows[charsettings].positionY;
	CreateBox(windows[charsettings]);
}
void Printer::Bar(std::string pretext,int index, int state, int max)
{
	int lastLine = windows[stopwatches].positionY+index;
	std::string bar = pretext+" |";
	for(int i = 0; i < max; ++i)
	{
		if(i != max/2)
		{
			if(i < state)
			{
				bar.append("=");
			}else {
				bar.append(".");
			}
		}else {
			if(i<10)
			{
				bar.append(" ");
			}
			bar.append(std::to_string(state));
		}
	}
	bar.append("|");
	bar.append("from: ");
	bar.append(std::to_string(max));
	screenbuffer[lastLine++] = (bar);
}
void Printer::Flush(){
	std::cout << "\033[H" << std::flush;
	//screenbuffer.clear();
}

void Printer::OpenFightScreen(Character& character, std::shared_ptr<Monster>& monster)
{
	int lastLine = windows[fight].positionY;
	screenbuffer[++lastLine] = ("--------------------------Player---------------------------");
	screenbuffer[++lastLine] = ("Name: "+character.Name() );
	screenbuffer[++lastLine] = ("LVL: "+std::to_string(character.Lvl()));
	screenbuffer[++lastLine] = ("Life: "+std::to_string(character.Life()));
	screenbuffer[++lastLine] = ("Atk: "+std::to_string(character.Atk()));

	screenbuffer[++lastLine] = ("--------------------------Monster--------------------------");
	screenbuffer[++lastLine] = ("Name: "+ *monster->GetName() );
	screenbuffer[++lastLine] = ("LVL: "+std::to_string(*monster->GetLevel()) );
	screenbuffer[++lastLine] = ("Life: "+std::to_string(*monster->GetLife()) );
	screenbuffer[++lastLine] = (linebreak);
}
void Printer::EventsList(std::vector<std::tuple<Character::CharEvent, std::function<void*()>>>& events, std::shared_ptr<Monster> currentMonster = nullptr)
{
	int lastLine = windows[eventlist].positionY;
	int height = windows[eventlist].height;
	const int eventlistLength = height>2?height-2:0;

	PassStringToBuffer(lastLine, "Eventqueue: ");
	if(!events.empty())
	{
		size_t eventsSize = events.size();
		for(size_t i = 0; i < eventsSize; ++i)
		{
			if(i<eventlistLength)
			{
				std::string eventDesc = "";
				eventDesc.append(std::to_string(i+1)+": ");
				switch(std::get<Character::CharEvent>(events.at(i)))
				{
					case Character::CharEvent::Fight:
					{
						if(i == 0 && currentMonster != nullptr)
						{
							eventDesc.append("[Fight] "+*currentMonster->GetName());
						}else{
							eventDesc.append("[Fight] Monster Encountered");
						}
						PassStringToBuffer(++lastLine, eventDesc);
					}
					break;
					case Character::CharEvent::Chest:
					{
						eventDesc.append("[DROP] Item found");
						PassStringToBuffer(++lastLine, eventDesc);
					}
					break;
					case Character::CharEvent::Encounter:
					{
					}
					break;
					case Character::CharEvent::Nothing:
					{
					}
					break;
				}
			}else {
				PassStringToBuffer(++lastLine, "Additional Events: "+std::to_string(events.size()-eventlistLength));
				break;
			}
		}
	}else
	{
		PassStringToBuffer(++lastLine, "No Events found");
	}
	windows[windowType::eventlist].width = 0; //this will force the width to be recalculated each frame
	CreateBox(windows[windowType::eventlist]);
}
void Printer::PrintSkillAnimation(Char::AnimationType type)
{
	if(windows[skillAnimation].isDrawn == false) return;

	std::string color = "\033[0m";
	if(type == Char::AnimationType::ATTACK)	color = "\033[38;2;100;0;0m";
	if(type == Char::AnimationType::HEAL)	color = "\033[38;2;0;100;0m";
	
	int lastLine = windows[skillAnimation].positionY;
	screenbuffer[lastLine] = linebreak;
	screenbuffer[++lastLine] = color;
	lastLine++;
	const float piShort = 3.141592F;

	const int resolution = 5;
	int xVal = 0;
	switch (type)
	{
		case Char::AnimationType::HEAL:
		{
			xVal = std::round((std::sin((float)windows[skillAnimation].animationframe/piShort))*resolution);
			//const int yVal = std::round((std::cos((float)skillAnimaionFrames/piShort)+1)*resolution*2);
			for(size_t yCoord = 0; yCoord < resolution; ++yCoord)
			{
				screenbuffer[lastLine].clear();
				std::string s = " ";
				if(xVal <= yCoord)
				{
					s = "█";
				}
				for(int c = 0;c < screenWidth-5; ++c)
				{
					screenbuffer[lastLine].append(s);
				}
				lastLine++;
			}
			color = "\033[0m";
			screenbuffer[lastLine] = color;
			screenbuffer[++lastLine] = linebreak;

			windows[skillAnimation].animationframe++;
			if(windows[skillAnimation].animationframe >= 20)
			{
				windows[skillAnimation].animatin_isRunning = false;
				windows[skillAnimation].isDrawn = false;
				windows[skillAnimation].animationframe = 0;
			}
		}
		break;
		case Char::AnimationType::ATTACK:
		{
			const double animSize = screenWidth/2.0f;
			const double anim = animSize - windows[skillAnimation].animationframe/(30.0f/screenWidth);
			xVal = round(animSize - anim);
			for(size_t yCoord = 0; yCoord < resolution; ++yCoord)
			{
				screenbuffer[lastLine].clear();
				for(int c = 0;c < screenWidth-5; ++c)
				{
					screenbuffer[lastLine].append(" ");
					if(c == xVal)
					{
						screenbuffer[lastLine].append("█");
					}
				}
				lastLine++;
			}
			color = "\033[0m";
			screenbuffer[lastLine] = color;
			screenbuffer[++lastLine] = linebreak;

			windows[skillAnimation].animationframe++;
			if(anim <= 0)
			{
				windows[skillAnimation].animatin_isRunning = false;
				windows[skillAnimation].isDrawn = false;
				windows[skillAnimation].animationframe = 0;
			}
		}
		break;
	}

}
void Printer::Help()
{
	int lastLine = windows[help].positionY;
	std::string text;
	for(auto commendsIter = Commands::commandsMap.begin();commendsIter != Commands::commandsMap.end(); ++commendsIter)
	{
		if((int)(text.length()+commendsIter->second.length()) >= screenWidth)
		{
			screenbuffer[++lastLine] = (text);
			text.clear();
		}
		text.append(commendsIter->second); 
		if(std::next(commendsIter, 1) != Commands::commandsMap.end())
		{
			text.append(" | ") ;
		}
	}
	screenbuffer[++lastLine] = (text);
}

void Printer::PrintScreen()
{
	//buffer to stdout
	if(screenbuffer.empty()) { return; }
	std::string text;
	
	for(size_t i = 0; i < screenbuffer.size(); ++i)
        {
		if((int)i > screenHeight-1)
		{
			break;
		}
		std::string linenumber = "";
		if(i<10)
		{
			linenumber = " "+std::to_string(i)+"|";
		}else{
			linenumber = std::to_string(i)+"|";
		}
		if(screenbuffer.at(i).size() > (size_t)screenWidth)
		{

			text.append(linenumber+screenbuffer.at(i).substr(0,screenWidth) + "\n");
		}else{
			for(size_t y = screenbuffer.at(i).size()-3; y < screenWidth; ++y)
			{
				text.append("\033[0K");
			}
			text.append(linenumber+screenbuffer.at(i) + "\n");
		}

        }
	for(size_t i = screenHeight - screenbuffer.size();i>1;--i)
	{
		text.append("\033[0K\n");
	}
	std::cout << text;
	for(auto iter = screenbuffer.begin(); iter != screenbuffer.end(); ++iter)
	{
		iter->clear();
		for(size_t spaces = 0; spaces < screenWidth-5; ++spaces)
		{
			iter->push_back(' ');
		}
	}
	//screenbuffer.clear();
}
/*
 * Pass string to buffer
 */
void Printer::PassStringToBuffer(int index, std::string additionalString)
{
	int bufferlineLength = screenbuffer[index].length();
	for(auto charIndex = 0; charIndex < bufferlineLength; ++charIndex)
	{
		if(screenbuffer[index][charIndex] != ' ') continue;
		if(additionalString[charIndex]    == ' ') continue;
		if(additionalString[charIndex]    == '\0') break;

		screenbuffer[index][charIndex] = additionalString[charIndex];
	}
	if(additionalString.length()<bufferlineLength)
	{
		screenbuffer[index][additionalString.length()] = ' ';
	}
	screenbuffer[index][bufferlineLength] = '\0';
}

/*
Create a Box around our window
*/
void Printer::CreateBox(window& _window)
{
	size_t bufferSize	= screenbuffer.size();
	size_t startIndexY	= _window.positionY;
	size_t startIndexX	= _window.positionX;
	size_t endIndexY	= startIndexY + _window.height;

	if(_window.width == 0)
	{
		for(size_t lineIndex = startIndexY; lineIndex < endIndexY; ++lineIndex)
		{
			if(lineIndex >= bufferSize) { break; }
			for(size_t charIndex = startIndexX; charIndex < screenbuffer[lineIndex].size(); ++charIndex)
			{
				if(screenbuffer[lineIndex][charIndex] <= 31) continue;

				if(screenbuffer[lineIndex][charIndex] != ' ' && charIndex > _window.width)
				{
					_window.width = charIndex;
				}
			}
			if(_window.width == 0) _window.width = screenbuffer[lineIndex].size();

			_window.width -= startIndexX;
		}
		_window.width++;
	}

	size_t endIndexX	= startIndexX + _window.width;
	if(endIndexY < startIndexY || endIndexX < startIndexX) return;
	for(size_t index = startIndexY; index <= endIndexY; ++index)
	{
		if(index >= bufferSize) { break; }
		if( index == startIndexY)
		{
			for(size_t x = startIndexX; x < endIndexX; ++x)
			{
				if(screenbuffer[index][x] == ' ') {
					screenbuffer[index][x] = '-';
				}
			}
			continue;
		}
		if( index == endIndexY){
			for(size_t x = startIndexX; x < endIndexX; ++x)
			{
				screenbuffer[index][x] = '-';
			}
			break;
		}
		screenbuffer[index][startIndexX-1] = '|';
		screenbuffer[index][endIndexX]	   = '|';
	}
}

void Printer::ToggleWindow(Printer::windowType type){
	windows[type].isDrawn = !windows[type].isDrawn;
}

