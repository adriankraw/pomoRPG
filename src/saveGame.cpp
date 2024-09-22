#pragma once
#include <_ctype.h>
#include <cstddef>
#include <cstring>
#include <fstream>
#include <ios>
#include <iostream>
#include <map>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

#include "Character.cpp"
#include "stopwatch.cpp"


class saveGame{
private:
	Character *character;	

	void GenerateSave();
	std::vector<stopwatch> stopwatchList;
public:
	saveGame();
	~saveGame();
	
	Character* Char(){return character;};

	enum SaveGameKeys{
		lvl, exp, expMultiplier, name
	};
	std::map<SaveGameKeys, std::string> SaveGameKeywords{
		{saveGame::SaveGameKeys::lvl, "lvl"},
		{saveGame::SaveGameKeys::exp, "exp"},
		{saveGame::SaveGameKeys::expMultiplier, "expMultiplier"},
		{saveGame::SaveGameKeys::name, "name"},
	};
	std::string GetKeyValue(SaveGameKeys);	
	std::vector<stopwatch>& GetStopWatchList() {return stopwatchList;};
	
	void Save(const saveGame::SaveGameKeys, const std::string);
	void Save();
	void Load();

	std::shared_ptr<stopwatch> GetStopwatchIndex(std::string name);
	std::shared_ptr<stopwatch> AddStopwatch(std::string name);
};

saveGame::saveGame() {
	character = new Character();
};

std::string saveGame::GetKeyValue(SaveGameKeys key)
{
	switch (key) {
		case saveGame::SaveGameKeys::lvl:
			return std::to_string(this->character->Lvl());
		break;
		case saveGame::SaveGameKeys::exp:
			return std::to_string(this->character->Exp());
		break;
		case saveGame::SaveGameKeys::name:
			return this->character->Name();
		break;
		case saveGame::SaveGameKeys::expMultiplier:
			return std::to_string(this->character->Expmultiplier());
	}
}
std::shared_ptr<stopwatch> saveGame::GetStopwatchIndex(std::string nameOfWatch)
{
	for(auto watch: stopwatchList)
	{
		if(watch.GetName() == nameOfWatch)
		{
			
			return std::make_shared<stopwatch>(watch);
		}
	}
	return NULL;
}
std::shared_ptr<stopwatch> saveGame::AddStopwatch(std::string nameOfWatch)
{
	stopwatchList.push_back(stopwatch(nameOfWatch, 0));
	return std::make_shared<stopwatch>(stopwatchList.back());
}

void saveGame::GenerateSave(){
	std::fstream saveFile("saveFile.txt", std::ios::in);
	//muss ich noch schreiben... will aber gerade nicht.
}

void saveGame::Save(const saveGame::SaveGameKeys Keyword, const std::string value) {	
	std::fstream saveFile;

	saveFile.open("saveFile.txt", std::ios::in);
	std::string line = "";
	std::string newFile = "";
	const std::string key = saveGame::SaveGameKeywords[Keyword].c_str();
	if(saveFile.is_open())
	{
		while(std::getline(saveFile, line))
		{
			if(line.find(key)!=line.npos)
			{
				std::string startingWith = line.substr(0, line.find(":::"));
				
				newFile.append(startingWith+":::");
				newFile.append(value);
				newFile.append("\n");
			}
			else{
				newFile.append(line+"\n");
			}
			line.clear();
		}
		saveFile.clear();
		saveFile.close();
	}

	saveFile.open("saveFile.txt", std::ios::out);
	if(saveFile.is_open())
	{
		saveFile << newFile;
		saveFile.close();
	}
};
void saveGame::Save()
{
	std::fstream saveFile;

	saveFile.open("saveFile.txt", std::ios::out);
	if(saveFile.is_open())
	{
		for (auto key : saveGame::SaveGameKeywords) {
			saveFile << key.second.c_str() << ":::" << this->GetKeyValue(key.first);
			saveFile << "\n";
		}
		saveFile.close();
	}	
	saveFile.clear();
	saveFile.open("timerList.txt");
	if(saveFile.is_open())
	{
		for(auto iterator = stopwatchList.begin(); iterator != stopwatchList.end(); ++iterator) 
		{
			saveFile << iterator->GetName();
			saveFile << ":::";
			saveFile << iterator->GetcurrentTime();
			saveFile << "\n";
		}
	}
	saveFile.close();
}
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
				
				if(startingWith.compare(SaveGameKeywords[SaveGameKeys::lvl]) == 0)
				{
					character->SetLvl(std::stoi(line.substr(line.find(":::")+3, line.length())));
				}
				if(startingWith.compare(SaveGameKeywords[SaveGameKeys::name]) == 0)
				{
					character->SetName(line.substr(line.find(":::")+3, line.length()));
				}
				if(startingWith.compare(SaveGameKeywords[SaveGameKeys::exp]) == 0)
				{
					character->SetExp(std::stoi(line.substr(line.find(":::")+3, line.length())));
				}
				if(startingWith.compare(SaveGameKeywords[SaveGameKeys::expMultiplier]) == 0)
				{
					character->SetExpMultiplier(std::stoi(line.substr(line.find(":::")+3, line.length())));
				}
			}
		}
		saveFile.close();
	}
	saveFile.clear();
	saveFile.open("timerList.txt");
	line = "";
	if(saveFile.is_open())
	{
		while (std::getline(saveFile, line)) 
		{
			if(line.find(":::") != 0)
			{
				int l = line.find(":::");
				const std::string startingWith = line.substr(0, l);
				std::string endingwith = line.substr(l+3);
				double time = std::stoi(endingwith);
				stopwatchList.push_back(stopwatch(startingWith, time));
			}
		}
	}
}

saveGame::~saveGame() {
}
