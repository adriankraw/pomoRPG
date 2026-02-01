#pragma once
#include "Skills.h"

class Frostblade : public Skills{
public:
	Frostblade(std::string, const int);
	Frostblade(Frostblade &&) = default;
	Frostblade(const Frostblade &) = default;
	Frostblade &operator=(Frostblade &&) = default;
	Frostblade &operator=(const Frostblade &) = default;
	~Frostblade() override;

	void Activate(Unit& character, std::shared_ptr<Unit> monster) override;

	int cost{2};	
private:

	const int fireballDmg{28};
};

Frostblade::Frostblade(std::string _name, const int _skillExp) {
	name = _name;
	expToLevel = _skillExp;
}

Frostblade::~Frostblade() {
}

void Frostblade::Activate(Unit& character, std::shared_ptr<Unit> monster){
	if(character.life > 0)
	{
		DmgEnemy(monster, fireballDmg);
	}
}
