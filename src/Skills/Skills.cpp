#pragma once 

#include "./../Character/Unit.h"
#include <stdexcept>
#include <string>
#pragma once
class Skills {
public:
	Skills(){};
	Skills(std::string, int = 60);
	virtual ~Skills(){};

	std::string name{};
	int expToLevel{0};
	int skillCost{1};
	virtual void activate(Unit* c, Unit* m);

protected:
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
void Skills::activate(Unit* c, Unit* m)
{
	//activate current Skill
	if(c->life <= 0)
		throw std::invalid_argument("Unit c has not enough life to activate a skill /ak");
	if(m->life <= 0)
		throw std::invalid_argument("Unit m has not enough life /ak");
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
