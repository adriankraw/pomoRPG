#pragma once

#include <string>
class Monster {
public:
	Monster(int);
	Monster(Monster &&) = default;
	Monster(const Monster &) = default;
	Monster &operator=(Monster &&) = default;
	Monster &operator=(const Monster &) = default;
	~Monster();

	std::string* GetName();
private:
	std::string name;
	int life;
};

Monster::Monster(int _life)
{
	name = "SleepyDeveloper";
	life = _life;
}

Monster::~Monster() {
}
std::string* Monster::GetName()
{
	return &name;
}
