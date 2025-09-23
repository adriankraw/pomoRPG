#include <cstddef>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

#include "render.cpp"
#include "Character.cpp"

class printer{
public:
	printer();
	printer(printer &&) = default;
	printer(const printer &) = default;
	printer &operator=(printer &&) = default;
	printer &operator=(const printer &) = default;
	~printer();

	render *ren;

	void header();
	void timer();
	void characterStats(Character*);
	void flush();
	void Bar(std::string, int, int);
	void OpenFightScreen(Character*, Monster*);
	void EventsList(std::vector<std::tuple<Character::CharEvent, void*>>* events);
	void Circle(int);
	void printScreen();
	void setSize(int w, int h){
		screenWidth = w;
		screenHeight = h;
	}

private:
	std::vector<std::string> screenbuffer;
	std::string linebreak = "";
	int screenWidth = 0;
	int screenHeight = 0;
};

printer::printer() {
	ren = new render();
	for(size_t i = 0; i < 60; ++i)
	{
		linebreak.append("_");
	}
}

printer::~printer() {
	delete ren;
}

void printer::header() {

	screenbuffer.push_back("PomoRPG:"+std::to_string(screenWidth)+":"+std::to_string(screenHeight));
	screenbuffer.push_back(linebreak);
}
void printer::timer(){
	screenbuffer.push_back("\033[1m");
	for(int i = 0; i < (ren->resultpointer)->size(); ++i)
	{
		screenbuffer.push_back((*ren->resultpointer)[i]);
	}
	screenbuffer.push_back("\033[0m");
	screenbuffer.push_back(linebreak);
}
void printer::characterStats(Character* character){
	screenbuffer.push_back("RPG:");

	screenbuffer.push_back("Name \t"+character->Name());
	screenbuffer.push_back("ATK \t"+std::to_string(character->Atk()));
	screenbuffer.push_back("Def \t"+std::to_string(character->Def()));
	screenbuffer.push_back("LVL \t"+std::to_string(character->Lvl()));
	screenbuffer.push_back("Exp \t"+std::to_string(character->Exp())+"/"+std::to_string(character->GetNextLevelExp()));
	screenbuffer.push_back("ExpMul \t"+std::to_string(character->Expmultiplier()));

	for(auto& s: character->skillList)
	{
		screenbuffer.push_back("Skill: "+ s.name+ std::to_string(s.expToLevel));
	}

	screenbuffer.push_back(linebreak);
}
void printer::Bar(std::string pretext, int state, int max)
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
	screenbuffer.push_back(bar);
}
void printer::flush(){
	std::cout << "\033[H" << std::flush;
	screenbuffer.clear();
}

void printer::OpenFightScreen(Character* character, Monster* monster)
{
	screenbuffer.push_back("--------------------------Player---------------------------");
	screenbuffer.push_back("Name: "+character->Name() );
	screenbuffer.push_back("LVL: "+std::to_string(character->Lvl()));
	screenbuffer.push_back("Life: "+std::to_string(character->Life()));
	screenbuffer.push_back("Atk: "+std::to_string(character->Atk()));

	screenbuffer.push_back("--------------------------Monster--------------------------");
	screenbuffer.push_back("Name: "+ *monster->GetName() );
	screenbuffer.push_back("LVL: "+std::to_string(*monster->GetLevel()) );
	screenbuffer.push_back("Life: "+std::to_string(*monster->GetLife()) );
	screenbuffer.push_back(linebreak);
}
void printer::EventsList(std::vector<std::tuple<Character::CharEvent, void*>>* events)
{
	screenbuffer.push_back(linebreak);
	screenbuffer.push_back("MonsterList: ");
	if(events->size()>0)
	{
		for(int i = 0; i < events->size(); ++i)
		{
			if(i<10)
			{
				if(std::get<0>(events->at(i)) == Character::CharEvent::Fight)
				{
					std::string eventDesc = "";
					Monster* monster = (Monster*)(std::get<1>(events->at(i)));
					eventDesc.append("[LVL:"+std::to_string(*monster->GetLevel())+"] ");
					eventDesc.append(*monster->GetName());
					eventDesc.append(" Life:"+std::to_string(*monster->GetLife())+"/"+std::to_string(*monster->GetMaxLife()));
					screenbuffer.push_back(eventDesc);
				}
			}else {
				screenbuffer.push_back("Additional Events: "+std::to_string(events->size()-10));
				break;
			}
		}
	}else
	{
		screenbuffer.push_back("No Events found");
	}
	screenbuffer.push_back(linebreak);
}
void printer::Circle(int frame)
{

	int y_val = frame;
	y_val = std::round((std::sin(y_val/3.141592f)+1)*2.5);

	for (float y = 0; y <= 5; ++y)
	{
		int i  = frame;
		float x = std::round((std::cos(i/3.141592f)+1)*5);
		if (x >= 0)
		{
			for (float j = 0; j < x; ++j)
			{
				std::cout << " ";
			}
			if(y == y_val)
				std::cout << "X";
			std::cout << "\n";

		}
	}
}
void printer::printScreen()
{
	if(screenbuffer.size() == 0) return;
	std::string text = "";

	for(size_t i = 0; i < screenbuffer.size(); ++i)
        {
		if(i > screenHeight-1)
		{
			break;
		}
		if(screenbuffer.at(i).size() > screenWidth)
		{
			text.append(screenbuffer.at(i).substr(0,screenWidth) + "\n");
		}else{
			for(int y = screenbuffer.at(i).size(); y < screenWidth; ++y)
			{
				text.append("\033[0K");
			}
			text.append(screenbuffer.at(i) + "\n");
		}

        }
	for(int i = screenHeight - screenbuffer.size();i>1;--i)
	{
		text.append("\033[0K\n");
	}
	std::cout << text;
	screenbuffer.clear();
}
