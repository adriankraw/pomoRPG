#pragma once

#include <_stdlib.h>
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

	void SetName(const std::string&);
	void SetLvl(const int&);
	void SetExp(const int&);
	void SetExpMultiplier(const int&);

	const std::string Name(){return Character::name;}
	const int Lvl(){return Character::lvl;}
	const int Exp(){return Character::exp;}
	const int Life(){return Character::life;}
	const int Atk(){return 2;}
	const int Def(){return 2;}
	const int Expmultiplier(){ return Character::expMultiplier;}

	void GetAttacked(int value) override {};
	void GetLife(int value) override {};
	void AddLife(int);
	void levelUp();
	int GetNextLevelExp();
	void addLevelUpAction(void(*)());
	CharEvent GetRandomEvent();
	Area* CurrentArea();
	void AddUserItem(int, int);

	int CheckInventoryFor(int);
	void AddToInventory(int, int);
	void RemoveFromInventory(int, int);

	void AddMonsterToEventMap(Character::CharEvent event, Monster* monster);
	void AddUserItemToEventMap(Character::CharEvent event, ItemDrop* itemDrop);
	std::vector<std::tuple<Character::CharEvent, void*>>* GetEvents();
	std::vector<Skills> skillList{};

private:
	std::string name;
	int lvl;
	int exp;
	int expMultiplier;
	int life;
	std::map<int, int> inventory;
	std::vector<void (*)()> levelupActions;
	Area currentArea = Area();

	std::vector<std::tuple<Character::CharEvent, void*>> EventVector;

};
Character::Character(){};

Character::Character(const std::string _name, const int _lvl, const int _exp, const int _expMultiplier):
	name(_name), 
	lvl(_lvl),
	exp(_exp),
	expMultiplier(_expMultiplier),
	life(lvl*10){
}

void Character::SetName(const std::string &_name) {
	Character::name = _name;
}
void Character::SetLvl(const int &_lvl) {
	Character::lvl = _lvl;
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
	if(r <= 5) return Character::CharEvent::Encounter;
	if(r <= 20) return Character::CharEvent::Chest;
	if(r <= 70) return Character::CharEvent::Fight;
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
	return 0;
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
