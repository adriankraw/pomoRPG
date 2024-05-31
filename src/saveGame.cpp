#include <_ctype.h>
#include <fstream>
#include <iostream>
#include <string>

#include "Character.cpp"

class saveGame{
public:
	saveGame();
	saveGame(saveGame &&) = default;
	saveGame(const saveGame &) = default;
	saveGame &operator=(saveGame &&) = default;
	saveGame &operator=(const saveGame &) = default;
	~saveGame();
	
	void Save();
	void Load();
	Character Char(){return saveGame::character;}
private:
	Character character = Character();	
};

saveGame::saveGame() {};

void saveGame::Save() {};
void saveGame::Load() {

	std::ifstream saveFile;
	saveFile.open("saveFile.txt");
	std::string line("");
	if(saveFile.is_open())
	{
		while (std::getline(saveFile, line))
		{
			if(line.find(":::") != 0)
			{
				const std::string startingWith = line.substr(0, line.find(":::"));
				
				if(startingWith.compare("LVL") == 0)
				{
					character.SetLvl(std::stoi(line.substr(line.find(":::")+3, line.length())));
				}
				if(startingWith.compare("name") == 0)
				{
					character.SetName(line.substr(line.find(":::")+3, line.length()));
				}
				if(startingWith.compare("exp") == 0)
				{
					character.SetExp(std::stoi(line.substr(line.find(":::")+3, line.length())));
				}
			}
		}
		saveFile.close();
	}
}

saveGame::~saveGame() {
}
