#include <iomanip>
#include <iostream>

#include "render.cpp"
#include "Character.cpp"
#include "Game.cpp"

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
	void Bar(std::string, int&);
	void OpenFightScreen(Character*, Area*, Monster*);

private:
};

printer::printer() {
	ren = new render();
}

printer::~printer() {
	delete ren;
}

void printer::header() {

	std::cout << std::left << "PomoRPG: "<< std::endl;
	std::cout << std::setw(80) << std::setfill('_') << '_' << std::endl << std::endl;
}
void printer::timer(){
	std::cout << "\033[1m";
	for(int i(0); i < (ren->resultpointer)->size(); ++i)
	{
		std::cout << (*ren->resultpointer)[i] << std::endl;
	}
	std::cout << "\033[0m";

	std::cout << std::setw(80) << std::setfill('_') << '_' << std::endl << std::endl;
}
void printer::characterStats(Character* character){
	std::cout << "RPG:" << std::endl << std::endl;

	std::cout << "Name \t" << character->Name() << std::endl;
	std::cout << "LVL \t" << character->Lvl() << std::endl;
	std::cout << "Exp \t" << character->Exp() << "/" << character->GetNextLevelExp() << std::endl;
	std::cout << "ExpMul \t" << character->Expmultiplier() << std::endl;

	std::cout << std::setw(80) << std::setfill('_') << '_' << std::endl << std::endl;
}
void printer::Bar(std::string pretext, int &state)
{
	std::cout << pretext << " |";
	for(int i = 0; i < 60; ++i)
	{
		if(i != 29)
		{
			if(i < state)
			{
				std::cout << "=";
			}else {
				std::cout << ".";
			}
		}else {
			if(i<10)
			{
				std::cout << " ";
			}
			std::cout << state;
		}
	}
	std::cout << "|" << std::endl;
}
void printer::flush(){
	std::cout << "\033[1J \033[1H" << std::flush;
}

void printer::OpenFightScreen(Character* character, Area* area, Monster* monster)
{
	;
}
