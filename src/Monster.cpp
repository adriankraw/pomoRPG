#pragma once

#include <string>
#include "Character/Unit.h"
class Monster : public Unit{
public:
	Monster(int);
	Monster(Monster &&) = default;
	Monster(const Monster &) = default;
	Monster &operator=(Monster &&) = default;
	Monster &operator=(const Monster &) = default;
	~Monster();

	std::string* GetName();
	int* GetLife();
	int* GetMaxLife();
	int* GetLevel();
	void GetAttacked(int value) override;
	void GetLife(int value) override{};
	
private:
	std::string name;
	int life;
	int maxLife;
	int level;
};

Monster::Monster(int _life)
{
	name = "SleepyDeveloper";
	life = _life;
	maxLife = _life;
	level = 1;
}

Monster::~Monster() {
}
std::string* Monster::GetName()
{
	return &name;
}
int* Monster::GetLife()
{
	return &life;
}
int* Monster::GetMaxLife()
{
	return &maxLife;
}
int* Monster::GetLevel()
{
	return &level;
}
void Monster::GetAttacked(int dmg)
{
	if(life-dmg > 0)
	{
		life -= dmg;
	}else {
		life = 0;
		//kill monster
	}
}
