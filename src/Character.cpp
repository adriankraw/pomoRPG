#pragma once

#include <cmath>
#include <iostream>
#include <map>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

#include "Area.cpp"
#include "Monster.cpp"


class Character{
public:
	Character();
	Character(std::string, int, int, int);
	Character(Character &&) = default;
	Character(const Character &) = default;
	Character &operator=(Character &&) = default;
	Character &operator=(const Character &) = default;


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
	const int Expmultiplier(){ return Character::expMultiplier;}

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

	void AddMonsterToEventMap(Character::CharEvent, Monster*);
	std::vector<std::tuple<Character::CharEvent, void*>>* GetEvents();

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
	return Character::CharEvent::Fight;
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

std::vector<std::tuple<Character::CharEvent, void*>> * Character::GetEvents()
{
	return &(Character::EventVector);
}
