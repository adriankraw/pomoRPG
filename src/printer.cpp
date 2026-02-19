#pragma once

#include <cassert>
#include <cmath>
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

	bool print_bigClock = true;
	int print_bigClockX = 0;
	int print_bigClockY = 1;
	bool print_charsettings = false;
	int print_charsettingsX = 0;
	int print_charsettingsY = 9;
	bool print_input = true;
	int print_inputX = 0;
	int print_inputY = 0; //special case
	bool print_stopwatches = false;
	int print_stopwatchX = 0;
	int print_stopwatchY = 22;
	bool print_eventList = false;
	int print_eventListX = 0;
	int print_eventListY = 27;
	bool print_fight = false;
	int print_fightX = 0;
	int print_fightY = 32;
	bool print_circle = false;
	int print_circleX = 0;
	int print_circleY = 50;
	bool skillAnimation_isRunning = false;
	int skillAnimaionFrames = 0;
	bool print_help = false;
	int print_helpX = 0;
	int print_helpY = 45;

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
			iter->push_back(' ');
		}
	}
}

Printer::~Printer() {
	screenbuffer.clear();
	std::cout << std::flush;
}

void Printer::CalcLineBreak(){
	linebreak = "";
	if(screenHeight> 5)
	screenbuffer.resize(screenHeight-2," ");

	if(screenWidth > 5)
	for(size_t i = 0; i < (screenWidth-5); ++i)
	{
		linebreak.append("-");	
	}
}
void Printer::Header() {
	int lastLine = 0;
	screenbuffer[lastLine] = "PomoRPG:"+std::to_string(screenWidth)+":"+std::to_string(screenHeight);
	screenbuffer[++lastLine] = linebreak;
}
void Printer::Timer(Time &currentTime){
	int lastLine = print_bigClockY;
	ren.renderTime(currentTime);
	screenbuffer[++lastLine] = "\033[1m";
	size_t resultSize = ren.result.size();
	for(size_t i = 0; i < resultSize; ++i)
	{
		screenbuffer[++lastLine] = (ren.result[i]);
	}
	screenbuffer[++lastLine] = "\033[0m";
	screenbuffer[++lastLine] = linebreak;
}
void Printer::CharacterStats(Character& character){
	int lastLine = print_charsettingsY;
	screenbuffer[++lastLine]= "RPG:";

	screenbuffer[++lastLine] = "Name \t"+character.Name();
	screenbuffer[++lastLine] = "LIFE \t"+std::to_string(character.Life());
	screenbuffer[++lastLine] = "ATK \t"+std::to_string(character.Atk());
	screenbuffer[++lastLine] = "Def \t"+std::to_string(character.Def());
	screenbuffer[++lastLine] = "LVL \t"+std::to_string(character.Lvl());
	screenbuffer[++lastLine] = "Exp \t"+std::to_string(character.Exp())+"/"+std::to_string(character.GetNextLevelExp());
	screenbuffer[++lastLine] = "ExpMul \t"+std::to_string(character.Expmultiplier());
	int skillcount = 0;	
	for(auto& skill: character.skillList)
	{
		screenbuffer[++lastLine] = "Skill: "+ skill->name +":"+ std::to_string(skill->expToLevel)+" cost:"+std::to_string(skill->cost);
		skillcount++;
	}

	screenbuffer[++lastLine] = linebreak;
}
void Printer::Bar(std::string pretext,int index, int state, int max)
{
	int lastLine = print_stopwatchY+index;
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
	screenbuffer[++lastLine] = (bar);
}
void Printer::Flush(){
	std::cout << "\033[H" << std::flush;
	//screenbuffer.clear();
}

void Printer::OpenFightScreen(Character& character, std::shared_ptr<Monster>& monster)
{
	int lastLine = print_fightY;
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
	int lastLine = print_eventListY;
	screenbuffer[++lastLine] = (linebreak);
	screenbuffer[++lastLine] = ("MonsterList: ");
	const int eventlistLength{10};
	if(!events.empty())
	{
		size_t eventsSize = events.size();
		for(size_t i = 0; i < eventsSize; ++i)
		{
			if(i<eventlistLength)
			{
				switch(std::get<Character::CharEvent>(events.at(i)))
				{
					case Character::CharEvent::Fight:
					{
						std::string eventDesc;
						if(i == 0 && currentMonster != nullptr)
						{
							eventDesc.append("[Fight] "+*currentMonster->GetName());
						}else{
							eventDesc.append("[Fight] Monster Encountered");
						}
						screenbuffer[++lastLine] = (eventDesc);
					}
					break;
					case Character::CharEvent::Chest:
					{
						std::string eventDesc;
						eventDesc.append("[DROP] Item found");
						screenbuffer[++lastLine] = (eventDesc);
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
				screenbuffer[++lastLine] = ("Additional Events: "+std::to_string(events.size()-eventlistLength));
				break;
			}
		}
	}else
	{
		screenbuffer[++lastLine] = ("No Events found");
	}
	screenbuffer[++lastLine] = (linebreak);
}
void Printer::PrintSkillAnimation(Char::AnimationType type)
{
	if(skillAnimation_isRunning == false) return;

	std::string color = "\033[0m";
	if(type == Char::AnimationType::ATTACK)	color = "\033[38;2;100;0;0m";
	if(type == Char::AnimationType::HEAL)	color = "\033[38;2;0;100;0m";
	
	int lastLine = print_circleY;
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
			xVal = std::round((std::sin((float)skillAnimaionFrames/piShort))*resolution);
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

			skillAnimaionFrames++;
			if(skillAnimaionFrames >= 20)
			{
				skillAnimation_isRunning = false;
				print_circle = false;
				skillAnimaionFrames = 0;
			}
		}
		break;
		case Char::AnimationType::ATTACK:
		{
			const double animSize = screenWidth/2.0f;
			const double anim = animSize - skillAnimaionFrames/(30.0f/screenWidth);
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

			skillAnimaionFrames++;
			if(anim <= 0)
			{
				skillAnimation_isRunning = false;
				print_circle = false;
				skillAnimaionFrames = 0;
			}
		}
		break;
	}

}
void Printer::Help()
{
	int lastLine = print_helpY;
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
