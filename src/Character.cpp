#pragma once
#include <cmath>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>
#include "Game.cpp"


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
	const int Expmultiplier(){ return Character::expMultiplier;}

	void levelUp();
	int GetNextLevelExp();
	void addLevelUpAction(void(*)());
	CharEvent GetRandomEvent();
	Area* CurrentArea();
	void AddUserItem(int, int);

private:
	std::string name;
	int lvl;
	int exp;
	int expMultiplier;
	std::vector<void (*)()> levelupActions;
	Area currentArea = Area();
};
Character::Character(){};

Character::Character(const std::string _name, const int _lvl, const int _exp, const int _expMultiplier):
	name(_name), 
	lvl(_lvl),
	exp(_exp),
	expMultiplier(_expMultiplier){
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
