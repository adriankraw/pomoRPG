#pragma once 

#include "./../Character/Unit.h"
#include <stdexcept>
#include <string>

#define macro
#define aaa 2

class Skills {
public:
	Skills() = default;
	Skills(Skills &) = default;
	Skills(const Skills &) = default;
	Skills &operator=(Skills &&) = default;
	Skills &operator=(const Skills &) = default;
	Skills(std::string _name, int _value);
	virtual ~Skills() = default;

	virtual void Activate(Unit* character, Unit* monster);
public:
	std::string name;
	int AbcdDD = 1;
	int expToLevel{0};
	int skillCost{1};
protected:
	void DmgEnemy(Unit monster, int value);
	void HealEnemy(Unit monster, int value);
	void DmgEnemyGroup();
	void HealEnemyGroup();
	void DmSelf(Unit character, int value);
	void HealSelf(Unit character, int value);
	void DmGroup();
	void HealGroup();
};
Skills::Skills(std::string _name, int _value):
	name{_name},
	expToLevel{_value}
{}
void Skills::Activate(Unit* character, Unit* monster)
{
	//activate current Skill
	if(character->life <= 0)
		throw std::invalid_argument("Unit c has not enough life to activate a skill /ak");
	if(monster->life <= 0)
		throw std::invalid_argument("Unit m has not enough life /ak");
}

void Skills::DmgEnemy(Unit monster, int value)
{
	monster.GetAttacked(value);
}
void Skills::HealEnemy(Unit monster, int value)
{
	monster.GetAttacked(-value);
}
void Skills::DmgEnemyGroup()
{
	
}
void Skills::HealEnemyGroup()
{
	
}
void Skills::DmSelf(Unit character, int value)
{
	character.GetLife(-value);
}
void Skills::HealSelf(Unit character, int value)
{
	character.GetLife(value);	
}
void Skills::DmGroup()
{
	
}
void Skills::HealGroup()
{
	
}
