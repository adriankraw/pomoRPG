#pragma once 

#include "Character.cpp"
#include "Monster.cpp"

class Skills {
public:
	Skills(std::string);

	std::string name;
	void* activate;

	void Heal(Character);
	void Slice(Character, Monster);
	void Fireball(Character, Monster);

private:
	void DmgEnemy(Monster m, int value);
	void HealEnemy(Monster m, int value);
	void DmgEnemyGroup();
	void HealEnemyGroup();
	void DmSelf(Character c, int value);
	void HealSelf(Character c, int value);
	void DmGroup();
	void HealGroup();
};
Skills::Skills(std::string _name)
{
	name = _name;
}

void Skills::DmgEnemy(Monster m, int value)
{
	m.GetAttacked(value);
}
void Skills::HealEnemy(Monster m, int value)
{
	m.GetAttacked(-value);
}
void Skills::DmgEnemyGroup()
{
	
}
void Skills::HealEnemyGroup()
{
	
}
void Skills::DmSelf(Character c, int value)
{
	c.AddLife(-value);
}
void Skills::HealSelf(Character c, int value)
{
	c.AddLife(value);	
}
void Skills::DmGroup()
{
	
}
void Skills::HealGroup()
{
	
}

void Skills::Heal(Character c)
{
	HealSelf(c, 10);
}

void Skills::Slice(Character c, Monster m)
{
	m.GetAttacked(c.Atk());
}

void Skills::Fireball(Character c, Monster m)
{
	m.GetAttacked(c.Atk()*2);	
}
