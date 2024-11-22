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
#include "boost/date_time/time_defs.hpp"
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
	std::vector<stopwatch>* GetStopWatchList() {return &stopwatchList;};
	stopwatch* GetStopWatchByIndex(int i) {return &(stopwatchList[i]);};
	
	void Save(const saveGame::SaveGameKeys, const std::string);
	void Save();
	void Load();

	int GetStopwatchIndex(std::string name);
	stopwatch* AddStopwatch(std::string name);
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
int saveGame::GetStopwatchIndex(std::string nameOfWatch)
{
	int index = 0;
	for(auto watch: stopwatchList)
	{
		if(*watch.GetName() == nameOfWatch)
		{
			
			break;
		}
		++index;
	}
	return index;
}
stopwatch* saveGame::AddStopwatch(std::string nameOfWatch)
{
	stopwatchList.push_back(stopwatch(nameOfWatch));
	
	return saveGame::GetStopWatchByIndex(GetStopwatchIndex(nameOfWatch));
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
		saveFile.flush();
		saveFile.clear();
		saveFile.close();
	}

	saveFile.open("saveFile.txt", std::ios::out);
	if(saveFile.is_open())
	{
		saveFile << newFile;
		saveFile.flush();
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
			std::string text;
			text.append(key.second.c_str());
			text.append(":::");
			text.append(this->GetKeyValue(key.first));
			text.append("\n");
			saveFile << text;
			saveFile.flush();
		}
		saveFile.close();
	}	
	saveFile.clear();

	saveFile.open("timerList.txt");
	if(saveFile.is_open())
	{
		for(auto iterator = stopwatchList.begin(); iterator != stopwatchList.end(); ++iterator) 
		{
			std::string text;
			text.append(*iterator->GetName());
			text.append(":::");
			text.append(iterator->GetcurrentTimeAsString());
			text.append("\n");
			saveFile << text;
			saveFile.flush();
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
				std::cout << l << std::endl;
				const std::string startingWith = line.substr(0, l);
				std::cout << startingWith << std::endl;
				int ll = line.find(":::",l+3);
				std::cout << ll << std::endl;

				std::string timeStamp = line.substr(l+3,ll-l-3);
				std::cout << timeStamp << std::endl;
				int hIndex = timeStamp.find("h");
				int mIndex = timeStamp.find("m");
				int sIndex = timeStamp.find("s");
				int miliIndex = timeStamp.find("mili"); 

				int hour = std::stoi(timeStamp.substr(0,hIndex));
				int minute = std::stoi( timeStamp.substr(hIndex+1, mIndex-hIndex-1));
				int seconds = std::stoi(timeStamp.substr(mIndex+1, sIndex-mIndex-1));
				int mili = std::stoi(timeStamp.substr(sIndex+1, timeStamp.length()-miliIndex-3));

				std::string startState = line.substr(ll+3);
				stopwatch nextWatch(startingWith, Time(hour,minute,seconds,mili));
				if(startState == "stopped")
				{
					nextWatch.GetTimer()->Pause();
				}
				stopwatchList.push_back(nextWatch);
			}
		}
	}
}

saveGame::~saveGame() {
}
