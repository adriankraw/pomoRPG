#pragma once
#include <cstddef>
#include <cstring>
#include <fstream>
#include <ios>
#include <iostream>
#include <map>
#include <ostream>
#include <string>
#include <vector>

#include "Character.cpp"
#include "stopwatch.cpp"
#include "Skills.cpp"
#include "logger.cpp"

logger saveGameLogger("saveGame.log");

class saveGame{
private:
	Character *character;	
	std::vector<stopwatch> stopwatchList;
	std::vector<Skills> skillList;

	void GenerateSave();
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
	void LoadSaveGame(Character*);
	void LoadTimers(std::vector<stopwatch>*);
	void LoadSkills(std::vector<Skills>*);

	int GetStopwatchIndex(std::string name);
	int GetMaxFromStopwatchName(std::string s);
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
int saveGame::GetMaxFromStopwatchName(std::string s)
{
	int max = 60;
	for (auto sw: skillList)
	{
		if (sw.name == s)
		{
			max = sw.expToLevel;
		}
	}
	return max;
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
			text.append(":::");
			if(iterator->GetTimer()->isPaused)
			{
				text.append("stopped");
			}else
			{
				text.append("started");
			}
			text.append("\n");
			saveFile << text;
			text = "";
			saveFile.flush();
		}
	}
	saveFile.close();
}
void saveGame::Load() {
	LoadSaveGame(character);
	LoadSkills(&skillList);
	LoadTimers(&stopwatchList);
}
void saveGame::LoadSaveGame(Character* character)
{
	std::ifstream saveFile;
	saveFile.open("saveFile.txt");
	std::string line("");
	if(saveFile.is_open())
	{
		while (std::getline(saveFile, line))
		{
			if(line == "") continue;

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
}
void saveGame::LoadTimers(std::vector<stopwatch>* stopwatchList)
{
	std::ifstream saveFile;
	saveFile.open("timerList.txt");
	std::string line("");
	if(saveFile.is_open())
	{
		while (std::getline(saveFile, line)) 
		{
			if(line == "") continue;

			if(line.find(":::") != 0)
			{
				int l = line.find(":::");
				const std::string startingWith = line.substr(0, l);
				int ll = line.find(":::",l+3);
				int lll = line.find(":::",ll+3);

				std::string timeStamp = line.substr(l+3,ll-l-3);
				int hIndex = timeStamp.find("h");
				int mIndex = timeStamp.find("m");
				int sIndex = timeStamp.find("s");
				int miliIndex = timeStamp.find("mili"); 

				int hour = std::stoi(timeStamp.substr(0,hIndex));
				int minute = std::stoi( timeStamp.substr(hIndex+1, mIndex-hIndex-1));
				int seconds = std::stoi(timeStamp.substr(mIndex+1, sIndex-mIndex-1));
				int mili = std::stoi(timeStamp.substr(sIndex+1, timeStamp.length()-miliIndex-3));

				const std::string startState = line.substr(ll+3, lll-ll-3);

				const std::string skillBind = line.substr(lll+3);
				
				stopwatch nextWatch(startingWith, Time(hour,minute,seconds,mili), skillBind);
				if(startState == "stopped")
				{
					nextWatch.GetTimer()->Pause();
				}
				stopwatchList->push_back(nextWatch);
			}
		}
	}
	saveFile.clear();
}
void saveGame::LoadSkills(std::vector<Skills>* skillList)
{
	std::ifstream saveFile;
	std::string line("");
	saveFile.open("skilltree.txt");
	line = "";
	if(saveFile.is_open())
	{
		while(std::getline(saveFile, line))
		{
			if(line == "") continue;

			if(line.find(":::") != 0)
			{
				int l = line.find(":::");
				const std::string startingWith = line.substr(0, l);
				const int skillExp = std::stoi(line.substr(l+3, line.length()-l));
				//saveGameLogger.log(logger::ErrorLevel::Info, startingWith + " " + std::to_string(skillExp));
				skillList->push_back(Skills(startingWith, skillExp));
				//map Skill + timer to array in Char
				
			}
		}
	}
	saveFile.clear();
}

saveGame::~saveGame() {
}
