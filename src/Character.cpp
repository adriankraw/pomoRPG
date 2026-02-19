#pragma once

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <map>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

#include "Area.cpp"
#include "Monster.cpp"
#include "Skills/Skills.h"
#include "./Eventable/ItemDrop.h"
#include "logger.cpp"

logger charLogger("character.log");

class Character: public Unit{
public:
	Character();
	Character(std::string, int, int, int);
	Character(Character &&) = default;
	Character(const Character &) = default;
	Character &operator=(Character &&) = default;
	Character &operator=(const Character &) = default;
	~Character() {};	

	enum CharEvent
	{
		Fight,
		Chest,
		Encounter,
		Nothing
	};



	void SetName(const std::string& _name);
	void SetLvl(const int& _lvl);
	void SetExp(const int& _exp);
	void SetExpMultiplier(const int& _expMulti);

	std::string Name() const { return name;}
	int Lvl() const { return lvl;}
	int Exp() const { return exp;}
	int Life() const { return life;}
	int Atk() const { return 2;}
	int Def() const { return 2;}
	int Expmultiplier() const { return expMultiplier;}

	void GetAttacked(int value) override;
	void GetLife(int value) override;
	void AddLife(int);
	void levelUp();
	int GetNextLevelExp();
	void addLevelUpAction(void(*)());
	CharEvent GetRandomEvent();
	Area* CurrentArea();
	void AddUserItem(int, int);
	Skills* GetSkill();

	int CheckInventoryFor(int);
	void AddToInventory(int, int);
	void RemoveFromInventory(int, int);

	void AddMonsterToEventMap(Character::CharEvent event, const Monster& monster);
	void AddUserItemToEventMap(Character::CharEvent event, const ItemDrop& itemDrop);
	auto& GetEvents();

	int currentSkill = 0;
	Char::AnimationType skillAnimationType = Char::AnimationType::ATTACK;
	std::vector<Skills*> skillList{};
	int skillCooldown{1};

private:
	std::string name;
	int lvl;
	int exp;
	int expMultiplier;
	int maxLife;
	std::map<int, int> inventory;
	std::vector<void (*)()> levelupActions;
	Area currentArea = Area{};

	std::vector<std::tuple<Character::CharEvent, std::function<void*()>>> EventVector{}; // change into tuple<CharEvent, EventStruct> struct should contain all the information for the Event to be triggered

};
Character::Character(){
	charLogger.log(logger::ErrorLevel::Warn, "baseConstructor used!");
};

Character::Character(const std::string _name, const int _lvl, const int _exp, const int _expMultiplier):
	name(_name), 
	lvl(_lvl),
	exp(_exp),
	expMultiplier(_expMultiplier),
	maxLife(_lvl*10)
{
	life = _lvl*10;
	charLogger.log(logger::ErrorLevel::Info, std::to_string(life));
}

void Character::SetName(const std::string &_name) {
	name = _name;
}
void Character::SetLvl(const int &_lvl) {
	lvl = _lvl;
	life = _lvl*10;
	maxLife = _lvl*10;
}
void Character::SetExp(const int &_exp) {
	if(_exp >= GetNextLevelExp()){
		exp = _exp%GetNextLevelExp();
  		levelUp();
	}else{
		exp = _exp;
	}
}
void Character::SetExpMultiplier(const int &_expMulti)
{
	expMultiplier = _expMulti;
}
void Character::GetAttacked(int value)
{
	if(value >= 0)
		life -= value;
}
void Character::GetLife(int value)
{
	if(value >= 0)
	{
		life = std::min(life+value, maxLife);
	}
}
void Character::AddLife(int value)
{
	life += value;
}
void Character::levelUp(){
	++lvl;
	for (auto func: levelupActions) {
		func();
	}
	//save the char
}
void Character::addLevelUpAction(void (*action)())
{
	levelupActions.push_back(action);
}
int Character::GetNextLevelExp(){
	return std::pow(std::pow(lvl-1,2)+4,2);
}
Character::CharEvent Character::GetRandomEvent()
{
	float fightChance = 70;
	float chestChance = 20;
	float encounterChance = 5;

	int r = rand() % 100;	
	if(r <= encounterChance) return Character::CharEvent::Encounter;
	if(r <= chestChance) return Character::CharEvent::Chest;
	if(r <= fightChance) return Character::CharEvent::Fight;
	return Character::CharEvent::Nothing;
}

Area* Character::CurrentArea()
{
	return &(currentArea);
}

void Character::AddUserItem(int itemCode, int itemAmount)
{
	std::cout << itemCode << " " << itemAmount << std::endl;
}
int Character::CheckInventoryFor(int itemCode)
{
	if(itemCode >= 0)
	{
		return 0;
	}else{
		return -1;
	}
}
void Character::AddToInventory(int itemCode, int itemAmount)
{
	if(Character::inventory.find(itemCode) == Character::inventory.end())
	{
		Character::inventory.insert(std::pair<int, int>( itemCode, itemAmount));
	}else {
		Character::inventory[itemCode] = (Character::inventory[itemCode]+itemAmount);	
	}
}
void Character::RemoveFromInventory(int itemCode, int itemAmount)
{
	if(inventory.find(itemCode) != inventory.end())
	{
		if(inventory[itemCode] > itemAmount)
		{
			inventory[itemCode] -= itemAmount;
		} else if (inventory[itemCode] == itemAmount) {
			inventory.erase(inventory.find(itemCode));
		}
		
	}
}
void Character::AddMonsterToEventMap(Character::CharEvent event, const Monster& monster)
{
	EventVector.emplace_back(std::tuple(event, [monster](){
		return new Monster(monster);
	}));
	charLogger.log(logger::ErrorLevel::Dbg, "ListSize:" + std::to_string(EventVector.size()));
}
void Character::AddUserItemToEventMap(Character::CharEvent event, const ItemDrop& itemDrop)
{
	EventVector.emplace_back(std::tuple(event, [itemDrop](){
		return new ItemDrop(itemDrop);
	}));
	charLogger.log(logger::ErrorLevel::Dbg, "ListSize:" + std::to_string(EventVector.size()));
}

auto& Character::GetEvents()
{
	return EventVector;
}

Skills* Character::GetSkill()
{
	// skills should have something like a coldown based on moves. 
	// heal costs you 1 extra move
	// Firebal costs you 2 extra moves
	Skills* skill = skillList.at(currentSkill);
	charLogger.log(logger::ErrorLevel::Info, skill->name);;
	currentSkill = (currentSkill + 1)%skillList.size();
	return skill;
}
