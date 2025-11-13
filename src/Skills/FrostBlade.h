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

	void Activate(Unit* character, Unit* monster) override;
private:

	const int fireballDmg{28};
	int cost{2};	
};

Frostblade::Frostblade(std::string _name, const int _skillExp) {
	name = _name;
	expToLevel = _skillExp;
}

Frostblade::~Frostblade() {
}

void Frostblade::Activate(Unit* character, Unit* monster){
	if(character!= nullptr)
	{
		DmgEnemy(monster, fireballDmg);
	}
}
