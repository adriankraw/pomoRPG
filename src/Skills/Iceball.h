#pragma once
#include "Skills.h"

class Iceball : public Skills{
public:
	Iceball(std::string, const int);
	Iceball(Iceball &&) = default;
	Iceball(const Iceball &) = default;
	Iceball &operator=(Iceball &&) = default;
	Iceball &operator=(const Iceball &) = default;
	~Iceball() override;

	void Activate(Unit& character, std::shared_ptr<Unit> monster) override;
private:

	const int fireballDmg{15};
	int cost{1};	
};

Iceball::Iceball(std::string _name, const int _skillExp) {
	name = _name;
	expToLevel = _skillExp;
}

Iceball::~Iceball() {
}

void Iceball::Activate(Unit& character, std::shared_ptr<Unit> monster){
	if(character.life > 0)
	{
		DmgEnemy(monster, fireballDmg);
	}
}
