#pragma once 

#include "./../Character/Unit.h"
#include <string>

class Skills {
public:
	Skills(std::string, int = 60);

	std::string name;
	int expToLevel;
	void* activate;

	void Heal(Unit);
	void Slice(Unit, Unit, int);
	void Fireball(Unit, Unit, int);

private:
	void DmgEnemy(Unit m, int value);
	void HealEnemy(Unit m, int value);
	void DmgEnemyGroup();
	void HealEnemyGroup();
	void DmSelf(Unit c, int value);
	void HealSelf(Unit c, int value);
	void DmGroup();
	void HealGroup();
};
Skills::Skills(std::string _name, int _value)
{
	name = _name;
	expToLevel = _value;
}

void Skills::DmgEnemy(Unit m, int value)
{
	m.GetAttacked(value);
}
void Skills::HealEnemy(Unit m, int value)
{
	m.GetAttacked(-value);
}
void Skills::DmgEnemyGroup()
{
	
}
void Skills::HealEnemyGroup()
{
	
}
void Skills::DmSelf(Unit c, int value)
{
	c.GetLife(-value);
}
void Skills::HealSelf(Unit c, int value)
{
	c.GetLife(value);	
}
void Skills::DmGroup()
{
	
}
void Skills::HealGroup()
{
	
}

void Skills::Heal(Unit c)
{
	HealSelf(c, 10);
}

void Skills::Slice(Unit c, Unit m, int value)
{
	m.GetAttacked(value);
}

void Skills::Fireball(Unit c, Unit m, int value)
{
	m.GetAttacked(value);	
}
