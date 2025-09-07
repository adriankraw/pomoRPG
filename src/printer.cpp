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

private:
	std::vector<std::string> screenbuffer;
	std::string linebreak = "";
};

printer::printer() {
	ren = new render();
	for(size_t i = 0; i < 80; ++i)
	{
		linebreak.append("_");
	}
	linebreak.append("\n");
}

printer::~printer() {
	delete ren;
}

void printer::header() {

	screenbuffer.push_back("PomoRPG:");
	screenbuffer.push_back(linebreak);
}
void printer::timer(){
	screenbuffer.push_back("\033[1m");
	for(int i(0); i < (ren->resultpointer)->size(); ++i)
	{
		screenbuffer.push_back((*ren->resultpointer)[i]);
	}
	screenbuffer.push_back("\033[0m");
	screenbuffer.push_back(linebreak);
}
void printer::characterStats(Character* character){
	screenbuffer.push_back("RPG:\n");

	screenbuffer.push_back("Name \t"+character->Name());
	//std::cout << "ATK \t" << character->Atk() << "\n";
	//std::cout << "Def \t" << character->Def() << "\n";
	screenbuffer.push_back("LVL \t"+std::to_string(character->Lvl()));
	screenbuffer.push_back("Exp \t"+std::to_string(character->Exp())+"/"+std::to_string(character->GetNextLevelExp()));
	screenbuffer.push_back("ExpMul \t"+std::to_string(character->Expmultiplier()));

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
	std::cout << "\033[1J \033[1H" << std::flush;
	screenbuffer.clear();
}

void printer::OpenFightScreen(Character* character, Monster* monster)
{
	screenbuffer.push_back("--------------------------Player---------------------------\n");
	screenbuffer.push_back("Name: "+character->Name() );
	screenbuffer.push_back("LVL: "+std::to_string(character->Lvl()));
	screenbuffer.push_back("Life: "+std::to_string(character->Life()));
	screenbuffer.push_back("Atk: "+std::to_string(character->Atk()));

	screenbuffer.push_back("--------------------------Monster--------------------------\n");
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
	std::cout << std::setw(80) << std::setfill('_') << '_' << "\n" << "\n";
}
void printer::printScreen()
{
	for(size_t i = 0; i < screenbuffer.size(); ++i)
        {
		std::cout << screenbuffer.at(i) << "\n";
        }
	screenbuffer.clear();
}
