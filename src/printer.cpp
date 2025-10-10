#include <cstddef>
#include <iostream>
#include <string>
#include <vector>

#include "render.cpp"
#include "Character.cpp"
#include "Eventable/ItemDrop.cpp"
#include "Commands.cpp"

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
	void CharacterStats(Character* character);
	void Flush();
	void Bar(std::string, int, int);
	void OpenFightScreen(Character* character, Monster* monster);
	void EventsList(std::vector<std::tuple<Character::CharEvent, void*>>* events);
	void Circle(int);
	void Help();
	void PrintScreen();
	void SetSize(int width, int height){
		screenWidth = width;
		screenHeight = height;
	}

	bool print_help = false;
	bool print_bigClock = true;
	bool print_charsettings = false;
	bool print_input = true;
	bool print_stopwatches = false;
	bool print_eventList = false;
	bool print_fight = false;
	bool print_circle = false;

private:
	render ren{};
	std::vector<std::string> screenbuffer;
	std::string linebreak = "";
	int screenWidth = 0;
	int screenHeight = 0;
};

Printer::Printer() {
	for(size_t i = 0; i < 60; ++i)
	{
		linebreak.append("_");
	}
}

Printer::~Printer() {
}

void Printer::Header() {

	screenbuffer.emplace_back("PomoRPG:"+std::to_string(screenWidth)+":"+std::to_string(screenHeight));
	screenbuffer.emplace_back(linebreak);
}
void Printer::Timer(Time &currentTime){
	ren.renderTime(currentTime);
	screenbuffer.emplace_back("\033[1m");
	size_t resultSize = ren.result.size();
	for(size_t i = 0; i < resultSize; ++i)
	{
		screenbuffer.emplace_back(ren.result[i]);
	}
	screenbuffer.emplace_back("\033[0m");
	screenbuffer.emplace_back(linebreak);
}
void Printer::CharacterStats(Character* character){
	screenbuffer.emplace_back("RPG:");

	screenbuffer.emplace_back("Name \t"+character->Name());
	screenbuffer.emplace_back("LIFE \t"+std::to_string(character->Life()));
	screenbuffer.emplace_back("ATK \t"+std::to_string(character->Atk()));
	screenbuffer.emplace_back("Def \t"+std::to_string(character->Def()));
	screenbuffer.emplace_back("LVL \t"+std::to_string(character->Lvl()));
	screenbuffer.emplace_back("Exp \t"+std::to_string(character->Exp())+"/"+std::to_string(character->GetNextLevelExp()));
	screenbuffer.emplace_back("ExpMul \t"+std::to_string(character->Expmultiplier()));

	for(auto& s: character->skillList)
	{
		screenbuffer.emplace_back("Skill: "+ s->name+ std::to_string(s->expToLevel));
	}

	screenbuffer.emplace_back(linebreak);
}
void Printer::Bar(std::string pretext, int state, int max)
{
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
	screenbuffer.emplace_back(bar);
}
void Printer::Flush(){
	std::cout << "\033[H" << std::flush;
	screenbuffer.clear();
}

void Printer::OpenFightScreen(Character* character, Monster* monster)
{
	screenbuffer.emplace_back("--------------------------Player---------------------------");
	screenbuffer.emplace_back("Name: "+character->Name() );
	screenbuffer.emplace_back("LVL: "+std::to_string(character->Lvl()));
	screenbuffer.emplace_back("Life: "+std::to_string(character->Life()));
	screenbuffer.emplace_back("Atk: "+std::to_string(character->Atk()));

	screenbuffer.emplace_back("--------------------------Monster--------------------------");
	screenbuffer.emplace_back("Name: "+ *monster->GetName() );
	screenbuffer.emplace_back("LVL: "+std::to_string(*monster->GetLevel()) );
	screenbuffer.emplace_back("Life: "+std::to_string(*monster->GetLife()) );
	screenbuffer.emplace_back(linebreak);
}
void Printer::EventsList(std::vector<std::tuple<Character::CharEvent, void*>>* events)
{
	screenbuffer.emplace_back(linebreak);
	screenbuffer.emplace_back("MonsterList: ");
	const int eventlistLength{10};
	if(!events->empty())
	{
		size_t eventsSize = events->size();
		for(size_t i = 0; i < eventsSize; ++i)
		{
			if(i<eventlistLength)
			{
				switch(std::get<0>(events->at(i)))
				{
					case Character::CharEvent::Fight:
					{
						std::string eventDesc;
						auto* monster = (Monster*)(std::get<1>(events->at(i)));
						eventDesc.append("[LVL:"+std::to_string(*monster->GetLevel())+"] ");
						eventDesc.append(*monster->GetName());
						eventDesc.append(" Life:"+std::to_string(*monster->GetLife())+"/"+std::to_string(*monster->GetMaxLife()));
						screenbuffer.emplace_back(eventDesc);
					}
					break;
					case Character::CharEvent::Chest:
					{
						std::string eventDesc;
						eventDesc.append("[DROP:] ");
						ItemDrop* itemdrop = (ItemDrop*)(std::get<1>(events->at(i))); 
						eventDesc.append(" Itemcode:"+std::to_string(itemdrop->itemCode)+" amount:"+ std::to_string(itemdrop->itemAmount));
						screenbuffer.emplace_back(eventDesc);
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
				if(std::get<0>(events->at(i)) == Character::CharEvent::Fight)
				{
				}
			}else {
				screenbuffer.emplace_back("Additional Events: "+std::to_string(events->size()-10));
				break;
			}
		}
	}else
	{
		screenbuffer.emplace_back("No Events found");
	}
	screenbuffer.emplace_back(linebreak);
}
void Printer::Circle(int frame)
{
	const int resolution = 10;
	const float piShort = 3.141592F;
	std::string output;
	const int xVal = std::round((std::sin((float)frame/piShort)+1)*(resolution-1));
	const int yVal = std::round((std::cos((float)frame/piShort)+1)*resolution/2);

	for(size_t yCoord = 0; yCoord < resolution; ++yCoord)
	{
		if(yCoord!=yVal){
			screenbuffer.emplace_back("");
		}else {
			for (float j = 0; j < xVal; ++j)
			{
				output.append(" ");
			}
			output.append("X");
			screenbuffer.emplace_back(output);
		}
	}
}
void Printer::Help()
{
	std::string text;
	for(auto commendsIter = Commands::commandsMap.begin();commendsIter != Commands::commandsMap.end(); ++commendsIter)
	{
		if((int)(text.length()+commendsIter->second.length()) >= screenWidth)
		{
			screenbuffer.emplace_back(text);
			text.clear();
		}
		text.append(commendsIter->second); 
		if(std::next(commendsIter,1) != Commands::commandsMap.end())
		{
			text.append("|");
		}
	}
	screenbuffer.emplace_back(text);
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
		if(screenbuffer.at(i).size() > (size_t)screenWidth)
		{
			text.append(screenbuffer.at(i).substr(0,screenWidth) + "\n");
		}else{
			for(size_t y = screenbuffer.at(i).size(); y < screenWidth; ++y)
			{
				text.append("\033[0K");
			}
			text.append(screenbuffer.at(i) + "\n");
		}

        }
	for(size_t i = screenHeight - screenbuffer.size();i>1;--i)
	{
		text.append("\033[0K\n");
	}
	std::cout << text;
	screenbuffer.clear();
}
