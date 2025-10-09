#pragma once 

#include "./../Character/Unit.h"
#include <string>

class Skills {
public:
	Skills() = default;
	Skills(Skills &) = default;
	Skills(const Skills &) = default;
	Skills &operator=(Skills &&) = default;
	Skills &operator=(const Skills &) = default;
	Skills(std::string _name, int _value);
	
	virtual ~Skills() = default;

	virtual void Activate(Unit* character, Unit* monster) = 0;
public:
	std::string name;
	int expToLevel{0};
	int skillCost{1};
protected:
	void DmgEnemy(Unit* monster, int value);
	void HealEnemy(Unit* monster, int value);
	void DmgEnemyGroup();
	void HealEnemyGroup();
	void DmSelf(Unit* character, int value);
	void HealSelf(Unit* character, int value);
	void DmGroup();
	void HealGroup();
};
Skills::Skills(std::string _name, int _value):
	name{_name},
	expToLevel{_value}
{}

void Skills::DmgEnemy(Unit* monster, int value)
{
	monster->GetAttacked(value);
}
void Skills::HealEnemy(Unit* monster, int value)
{
	monster->GetAttacked(-value);
}
void Skills::DmgEnemyGroup()
{
	
}
void Skills::HealEnemyGroup()
{
	
}
void Skills::DmSelf(Unit* character, int value)
{
	character->GetLife(-value);
}
void Skills::HealSelf(Unit* character, int value)
{
	character->GetLife(value);	
}
void Skills::DmGroup()
{
	
}
void Skills::HealGroup()
{
	
}
