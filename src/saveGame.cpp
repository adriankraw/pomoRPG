#pragma once
#include "Character.cpp"
#include "Skilltree.cpp"
#include "stopwatch.cpp"
#include <cstddef>
#include <cstring>
#include <fstream>
#include <ios>
#include <iostream>
#include <map>
#include <string>
#include <vector>

namespace save_game{

	enum SaveGameKeys{
		lvl, exp, expMultiplier, name
	};
	std::map<SaveGameKeys, std::string> SaveGameKeywords{
		{SaveGameKeys::lvl, "lvl"},
		{SaveGameKeys::exp, "exp"},
		{SaveGameKeys::expMultiplier, "expMultiplier"},
		{SaveGameKeys::name, "name"},
	};

	std::string GetKeyValue(Character& character, SaveGameKeys key)
	{
		switch (key) {
			case SaveGameKeys::lvl:
				return std::to_string(character.Lvl());
			break;
			case SaveGameKeys::exp:
				return std::to_string(character.Exp());
			break;
			case SaveGameKeys::name:
				return character.Name();
			break;
			case SaveGameKeys::expMultiplier:
				return std::to_string(character.Expmultiplier());
		}
	}


	void GenerateSave(){
		std::fstream saveFile("saveFile.txt", std::ios::in);
		//muss ich noch schreiben... will aber gerade nicht.
	}

	void Save(const SaveGameKeys Keyword, const std::string value) {	
		std::fstream saveFile;

		saveFile.open("saveFile.txt", std::ios::in);
		std::string line = "";
		std::string newFile = "";
		const std::string key = SaveGameKeywords[Keyword].c_str();
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
	void Save(Character& character, std::vector<stopwatch>& stopwatchList)
	{
		std::fstream saveFile;

		saveFile.open("saveFile.txt", std::ios::out);
		if(saveFile.is_open())
		{
			for (auto key : SaveGameKeywords) {
				std::string text;
				text.append(key.second.c_str());
				text.append(":::");
				text.append(GetKeyValue(character, key.first));
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

	void LoadSaveGame(Character& character)
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
					
					if(startingWith == SaveGameKeywords[SaveGameKeys::lvl])
					{
						character.SetLvl(std::stoi(line.substr(line.find(":::")+3, line.length())));
					}
					if(startingWith == SaveGameKeywords[SaveGameKeys::name])
					{
						character.SetName(line.substr(line.find(":::")+3, line.length()));
					}
					if(startingWith == SaveGameKeywords[SaveGameKeys::exp])
					{
						character.SetExp(std::stoi(line.substr(line.find(":::")+3, line.length())));
					}
					if(startingWith == SaveGameKeywords[SaveGameKeys::expMultiplier])
					{
						character.SetExpMultiplier(std::stoi(line.substr(line.find(":::")+3, line.length())));
					}
				}
			}
			saveFile.close();
		}
		saveFile.clear();
	}
	void LoadTimers(std::vector<stopwatch>& stopwatchList)
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
					stopwatchList.push_back(nextWatch);
				}
			}
		}
		saveFile.clear();
	}
	void LoadSkills(std::vector<Skills*>& skillList, Skilltree& skillTree)
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
					auto& t = skillTree.skillvector.at(startingWith);
					skillList.push_back(t.contructor(startingWith,skillExp));
					//map Skill + timer to array in Char
					
				}
			}
		}
		saveFile.clear();
	}

	void LoadAreas(std::vector<Area>& areas)
	{
		areas.emplace_back(Area());
	}

}
