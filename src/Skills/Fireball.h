#pragma once
#include "Skills.h"

class Fireball : public Skills{
public:
	Fireball(std::string, const int);
	Fireball(Fireball &&) = default;
	Fireball(const Fireball &) = default;
	Fireball &operator=(Fireball &&) = default;
	Fireball &operator=(const Fireball &) = default;
	~Fireball() override;

	void Activate(Unit* character, Unit* monster) override;
private:

	const int fireballDmg{10};
	
};

Fireball::Fireball(std::string _name, const int _skillExp) {
	name = _name;
	expToLevel = _skillExp;
}

Fireball::~Fireball() {
}

void Fireball::Activate(Unit* character, Unit* monster){
	if(character!= nullptr)
	{
		DmgEnemy(monster, fireballDmg);
	}
}
