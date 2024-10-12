#pragma once

class Monster {
public:
	Monster(int);
	Monster(Monster &&) = default;
	Monster(const Monster &) = default;
	Monster &operator=(Monster &&) = default;
	Monster &operator=(const Monster &) = default;
	~Monster();

private:
	int life;
};

Monster::Monster(int _life)
{
	life = _life;
}

Monster::~Monster() {
}
