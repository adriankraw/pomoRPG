#pragma once

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <map>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

#include "Area.cpp"
#include "Monster.cpp"
#include "./Skills/Skills.cpp"
#include "./Eventable/ItemDrop.cpp"
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

	std::string Name() const { return Character::name;}
	int Lvl() const { return Character::lvl;}
	int Exp() const { return Character::exp;}
	int Life() const { return Character::life;}
	int Atk() const { return 2;}
	int Def() const { return 2;}
	int Expmultiplier() const { return Character::expMultiplier;}

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

	void AddMonsterToEventMap(Character::CharEvent event, Monster* monster);
	void AddUserItemToEventMap(Character::CharEvent event, ItemDrop* itemDrop);
	std::vector<std::tuple<Character::CharEvent, void*>>* GetEvents();
	int currentSkill = 0;
	std::vector<Skills*> skillList{};

private:
	std::string name;
	int lvl;
	int exp;
	int expMultiplier;
	int life;
	int maxLife;
	std::map<int, int> inventory;
	std::vector<void (*)()> levelupActions;
	Area currentArea = Area();

	std::vector<std::tuple<Character::CharEvent, void*>> EventVector;

};
Character::Character(){
	charLogger.log(logger::ErrorLevel::Warn, "baseConstructor used!");
};

Character::Character(const std::string _name, const int _lvl, const int _exp, const int _expMultiplier):
	name(_name), 
	lvl(_lvl),
	exp(_exp),
	expMultiplier(_expMultiplier),
	life(_lvl*10),
	maxLife(_lvl*10)
{
	charLogger.log(logger::ErrorLevel::Info, std::to_string(life));
}

void Character::SetName(const std::string &_name) {
	Character::name = _name;
}
void Character::SetLvl(const int &_lvl) {
	Character::lvl = _lvl;
	Character::life = _lvl*10;
	Character::maxLife = _lvl*10;
}
void Character::SetExp(const int &_exp) {
	if(_exp >= GetNextLevelExp()){
		Character::exp = _exp%GetNextLevelExp();
  		Character::levelUp();
	}else{
		Character::exp = _exp;
	}
}
void Character::SetExpMultiplier(const int &_expMulti)
{
	Character::expMultiplier = _expMulti;
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
	Character::life += value;
}
void Character::levelUp(){
	++lvl;
	for (auto func: Character::levelupActions) {
		func();
	}
	//save the char
}
void Character::addLevelUpAction(void (*action)())
{
	Character::levelupActions.push_back(action);
}
int Character::GetNextLevelExp(){
	return std::pow(std::pow(Character::lvl-1,2)+4,2);
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
	return &(Character::currentArea);
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
	if(Character::inventory.find(itemCode) != Character::inventory.end())
	{
		if(Character::inventory[itemCode] > itemAmount)
		{
			Character::inventory[itemCode] -= itemAmount;
		} else if (Character::inventory[itemCode] == itemAmount) {
			Character::inventory.erase(Character::inventory.find(itemCode));
		}
		
	}
}
void Character::AddMonsterToEventMap(Character::CharEvent event, Monster* monster)
{
	Character::EventVector.push_back(std::tuple<Character::CharEvent, void*>(event, monster));
}
void Character::AddUserItemToEventMap(Character::CharEvent event, ItemDrop* itemDrop)
{
	Character::EventVector.push_back(std::tuple<Character::CharEvent, void*>(event, itemDrop));
}

std::vector<std::tuple<Character::CharEvent, void*>> * Character::GetEvents()
{
	return &(Character::EventVector);
}

Skills* Character::GetSkill()
{
	// skills should have something like a coldown based on moves. 
	// heal costs you 1 extra move
	// Firebal costs you 2 extra moves
	Skills* skill = skillList.at(currentSkill);
	currentSkill = (currentSkill + 1)%skillList.size();
	return skill;
}
