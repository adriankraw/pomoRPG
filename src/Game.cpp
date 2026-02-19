#pragma once

#include <string>
#include <thread>
#include <vector>

#include "Area.cpp"
#include "Character.cpp"
#include "Skilltree.cpp"
#include "Timer.cpp"
#include "stopwatch.cpp"
#include "saveGame.cpp"
#include "printer.cpp"
#include "KeyCode.h"

struct Event{
	bool running = false;
	Character::CharEvent eventType = Character::CharEvent::Nothing;
	std::shared_ptr<Monster> monster;
};

struct Manager{
	const int frames = 30;
	const int actionTick = 1000;
	const int eventTick = 5000;
	int screenWidth = 0;
	int screenHeight = 0;

	Printer print{};
};

struct Materials{
	int wood{0};
	int stone{0};
	int copper{0};
	int iron{0};
	int marmor{0};
	int gold{0};
};

struct FrameData{
	double frame = 0;
	double exp = 0;
	double eventTimer = 0;
	double actionTimer = 0;
	double deltaTime = 0;
};

class Game {
public:
	Game();
	Game(Game &&) = default;
	Game(const Game &) = default;
	Game &operator=(Game &&) = default;
	Game &operator=(const Game &) = default;
	~Game();
	
	Manager gameManager{};
	FrameData frameData{};
	bool isRunning;
	std::shared_ptr<std::string> currentInput;
	Character player;
	Event currentEvent{};
	Time* globalTimer = new Time(1,0,0,0);
	std::vector<stopwatch> stopwatchList{};
	std::vector<Area> areas{};
	Skilltree skilltree{};

	void SaveGame();
	void LoadGame();
	void Start();
	void Update();

	void SetInput(std::shared_ptr<std::string> keyboadInput);
	void SetWindowSize(int width, int height);
	void ProcessInput();

	stopwatch& AddStopwatch(std::string nameOfWatch);
	stopwatch& GetStopWatchByIndex(int index);
	int GetStopwatchIndex(std::string nameOfWatch);
	int GetMaxFromStopwatchName(std::string name);

private:
	Area currentArea;
	std::vector<Area> Areas{
		Area(Area::AreaProps{.name="forest",	.level=0, .monsterLevel=1}),
		Area(Area::AreaProps{.name="cave",	.level=1, .monsterLevel=1}),
		Area(Area::AreaProps{.name="garden",	.level=2, .monsterLevel=2}),
	};
	Rarity rarity;
};

Game::Game() {
	currentArea = Area{};
}

Game::~Game() {

}
void Game::SaveGame(){
	save_game::Save(player, stopwatchList);
}
void Game::LoadGame(){
	save_game::LoadSaveGame(player);
	save_game::LoadTimers(stopwatchList);
	save_game::LoadSkills(player.skillList,skilltree);
}

void Game::SetInput(std::shared_ptr<std::string> keyboadInput){
	currentInput = keyboadInput;
}
void Game::Start(){
	LoadGame();
	isRunning = true;
}
void Game::Update(){
	std::chrono::time_point<std::chrono::system_clock> startFrame = std::chrono::system_clock::now();
	std::chrono::time_point<std::chrono::system_clock> endFrame;

	ProcessInput();

	gameManager.print.Flush();
	gameManager.print.SetSize(gameManager.screenWidth, gameManager.screenHeight);
	gameManager.print.Header();
	if(gameManager.print.print_bigClock)
	{
		gameManager.print.Timer(*globalTimer);
	}
	if(gameManager.print.print_charsettings)
	{
		gameManager.print.CharacterStats(player);
	}
	if(gameManager.print.print_stopwatches)
	{
		size_t stopWatchListSize = stopwatchList.size();
		for (size_t i = 0; i<stopWatchListSize; ++i) {
			std::string t = GetStopWatchByIndex(i).GetNameOfCorrespondingSkill();
			int maxCount = GetMaxFromStopwatchName(t);
			int seconds = GetStopWatchByIndex(i).GetcurrentTime().ConvertToSecondsForModulo(maxCount);			
			gameManager.print.Bar(*GetStopWatchByIndex(i).GetName(), i, seconds, maxCount);
		}
	}
	if(gameManager.print.print_eventList)
	{
		if(currentEvent.eventType == Character::CharEvent::Fight)
		{
			gameManager.print.EventsList(player.GetEvents(), currentEvent.monster);
		}else{
			gameManager.print.EventsList(player.GetEvents());
		}
	}
	if(gameManager.print.print_circle)
	{
		gameManager.print.PrintSkillAnimation(player.skillAnimationType);
	}
	if(gameManager.print.print_fight)
	{
		auto events = player.GetEvents();
		if(currentEvent.running && currentEvent.eventType == Character::CharEvent::Fight)
		{
			gameManager.print.OpenFightScreen(player, currentEvent.monster);
		}
	}
	if(gameManager.print.print_help)
	{
		gameManager.print.Help();
	}
	gameManager.print.PrintScreen();
	/* this has to be handled on a different Thread */
	if(gameManager.print.print_input)
	{
		std::cout << "> " << *currentInput <<"\033[48;5;255m \033[0m \033[0K";
	}
	std::cout << "\033[?25l"; //Make Cursor invisible

	//logic
	{
		size_t stopwatchListSize = stopwatchList.size();
		for (size_t i = 0; i<stopwatchListSize; ++i) {
			if(GetStopWatchByIndex(i).GetTimer()->isPaused == false)
			{
				GetStopWatchByIndex(i).GetTimer()->Tick(GetStopWatchByIndex(i).GetcurrentTime(), frameData.deltaTime);
			}

			if(frameData.eventTimer >= gameManager.eventTick) // every 5 seconds 
			{
				Character::CharEvent charEvent = player.GetRandomEvent();
				switch(charEvent) {
					case Character::CharEvent::Fight:
					{
						Area* area = player.CurrentArea();
						charLogger.log(logger::Info, *area->GetName());

						player.AddMonsterToEventMap(Character::CharEvent::Fight, *area->Getmonster());
						//gameManager.print.OpenFightScreen(player, area, monster); This doesnt make sense right now
					}
					break;
					case Character::CharEvent::Chest:
					{
						charLogger.logInfo("chest found");
						Area* area = player.CurrentArea();
						Rarity::Level rarity = area->GetRandomRarety();
						int itemCode{0}, itemAmount{0};
						area->RollItem(&rarity, itemCode, itemAmount);
						player.AddUserItemToEventMap(Character::CharEvent::Chest, ItemDrop(itemCode, itemAmount));
					}
					break;
					case Character::CharEvent::Encounter:
						//coud be: you encounter some new place;
						//This is how you are supposed to find the RaidBoss of this Area;
					break;
					case Character::CharEvent::Nothing:
						//PlaceHolder for Nothing was found, Good luck next Time;
						;
					break;
					default:
						charLogger.log(logger::Info, "char Event default triggered");
					break;
				}
				frameData.eventTimer = 0;
			}

		}
		if(gameManager.print.print_fight && frameData.actionTimer >= player.skillCooldown * gameManager.actionTick)
		{
			std::vector<std::tuple<Character::CharEvent, std::function<void*()>>>& events = player.GetEvents();
			if(gameManager.print.print_fight && !events.empty())
			{
				switch(std::get<Character::CharEvent>(events.at(0)))
				{
					case Character::CharEvent::Fight:
					{
						charLogger.logInfo("action triggered while fighting");
						if(!currentEvent.running)
						{
							auto event = std::get<std::function<void*()>>(events.at(0));
							Monster* monster;
							monster = (Monster*)event();
							currentEvent.eventType = Character::CharEvent::Fight;
							currentEvent.monster = std::make_shared<Monster>(*monster);
							currentEvent.running = true;
						}
						auto* skill = player.GetSkill();
						player.skillCooldown = skill->cost;

						skill->Activate(player, currentEvent.monster);
						player.skillAnimationType = skill->animationType;
						gameManager.print.print_circle = true;	
						gameManager.print.skillAnimation_isRunning = true;
						player.GetAttacked(*currentEvent.monster->GetLevel());

						if(*currentEvent.monster->GetLife() <= 0)
						{
							events.erase(events.begin());
							player.SetExp(player.Exp() + 100);
							currentEvent.eventType = Character::CharEvent::Nothing;
							currentEvent.monster = nullptr;
							currentEvent.running = false;
						}
					}
					break;
					case Character::CharEvent::Chest:
					{
						charLogger.logInfo("action triggered while chest");
						auto& event = std::get<std::function<void*()>>(events.at(0));
						ItemDrop* itemDrop;
						itemDrop = (ItemDrop*)event();
						player.AddToInventory(itemDrop->itemCode, itemDrop->itemAmount);
						player.skillCooldown = 1;
						delete itemDrop;

						events.erase(events.begin());
						currentEvent.eventType = Character::CharEvent::Nothing;
						currentEvent.monster = nullptr;
						currentEvent.running = false;
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

			}
			frameData.actionTimer = 0;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(1000/gameManager.frames));
		endFrame = std::chrono::system_clock::now();
		frameData.deltaTime = std::chrono::duration<double, std::milli>(endFrame-startFrame).count();
		
		frameData.frame += 1.0f;
		frameData.exp += frameData.deltaTime;
		frameData.eventTimer += frameData.deltaTime;
		frameData.actionTimer += frameData.deltaTime;
		if(frameData.exp >= 1000)
		{
			player.SetExp((int)(player.Exp()+frameData.exp/1000)*player.Expmultiplier());
			frameData.exp = 0;
		}
	}
}

void Game::SetWindowSize(int width, int height){
	gameManager.screenWidth = width;
	gameManager.screenHeight = height;	
	gameManager.print.SetSize(width, height);
}
void Game::ProcessInput()
{
	if(currentInput->back() == '\n')
	{
		std::string additionalInfo;
		currentInput->pop_back();
		if((*currentInput).find(KeyCode::Btn::Space) != std::string::npos)
		{
			additionalInfo = currentInput->substr(currentInput->find(KeyCode::Btn::Space)+1,currentInput->length());
			*currentInput = currentInput->substr(0,currentInput->find(KeyCode::Btn::Space));
		}if(*currentInput == Commands::commandsMap[Commands::help])
		{
			gameManager.print.print_help = !gameManager.print.print_help;
		}else if(*currentInput == Commands::commandsMap[Commands::up])
		{
			//screenTimer.SetState(TimerState::countUp);
		}else if(*currentInput == Commands::commandsMap[Commands::down])
		{
			//screenTimer.SetState(TimerState::countDown);
		}else if(*currentInput == Commands::commandsMap[Commands::save])
		{
			SaveGame();
		}else if(*currentInput == Commands::commandsMap[Commands::timer])
		{
			//select a Timer
			if(!additionalInfo.empty())
			{
				globalTimer = &GetStopWatchByIndex(GetStopwatchIndex(additionalInfo)).GetcurrentTime();	
			}
		}else if(*currentInput == Commands::commandsMap[Commands::add])
		{
			if(!additionalInfo.empty())
			{
				AddStopwatch(additionalInfo);
			}
		}else if(*currentInput == Commands::commandsMap[Commands::pause] || *currentInput == Commands::commandsMap[Commands::stop])
		{
			if(!additionalInfo.empty())
			{
				if((size_t)GetStopwatchIndex(additionalInfo) != stopwatchList.size())
				{
					GetStopWatchByIndex(GetStopwatchIndex(additionalInfo)).GetTimer()->Pause();
				}
			}else {
				//screenTimer.isPaused = true;
			}
		}else if(*currentInput == Commands::commandsMap[Commands::start] || *currentInput == Commands::commandsMap[Commands::resume])
		{
			if(!additionalInfo.empty())
			{
				if((size_t)GetStopwatchIndex(additionalInfo) != stopwatchList.size())
				{
					GetStopWatchByIndex(GetStopwatchIndex(additionalInfo)).GetTimer()->UnPause();
				}
			}else {
				//screenTimer.isPaused = false;
			}
		}else if(*currentInput == Commands::commandsMap[Commands::bigclock])
		{
			gameManager.print.print_bigClock = !gameManager.print.print_bigClock;
		}else if(*currentInput == Commands::commandsMap[Commands::charsettings] || *currentInput == Commands::commandsMap[Commands::charstats])
		{
			gameManager.print.print_charsettings = !gameManager.print.print_charsettings;
		}else if(*currentInput == Commands::commandsMap[Commands::stopwatches])
		{
			gameManager.print.print_stopwatches = !gameManager.print.print_stopwatches;
		}else if(*currentInput == Commands::commandsMap[Commands::fight])
		{
			gameManager.print.print_fight = !gameManager.print.print_fight;
		}else if(*currentInput == Commands::commandsMap[Commands::eventlist])
		{
			gameManager.print.print_eventList = !gameManager.print.print_eventList;
		}else if(*currentInput == Commands::commandsMap[Commands::circle])
		{
			gameManager.print.print_circle = !gameManager.print.print_circle;
			gameManager.print.skillAnimation_isRunning = true;
		}else if(*currentInput == Commands::commandsMap[Commands::exit])
		{
			isRunning = false;
		}
		currentInput->clear();

	}
}
stopwatch& Game::GetStopWatchByIndex(int index){
	return stopwatchList[index];
}
int Game::GetStopwatchIndex(std::string nameOfWatch)
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
int Game::GetMaxFromStopwatchName(std::string name)
{
	int max = 60;
	for (auto sw: player.skillList)
	{
		if (sw->name == name)
		{
			max = sw->expToLevel;
		}
	}
	return max;
}

stopwatch& Game::AddStopwatch(std::string nameOfWatch)
{
	stopwatchList.emplace_back(stopwatch(nameOfWatch));
	
	return stopwatchList.back();
}
