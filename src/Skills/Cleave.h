#pragma once
#include "Skills.h"

class Cleave: public Skills {
public:
	Cleave(std::string, const int);
	Cleave(Cleave &&) = default;
	Cleave(const Cleave &) = default;
	Cleave &operator=(Cleave &&) = default;
	Cleave &operator=(const Cleave &) = default;
	~Cleave() override;

	void Activate(Unit* character, Unit* monster) override;

private:

	const int sliceDmg{31};
	int cost{3};
	
};

Cleave::Cleave(std::string _name, const int _skillExp) {
	name = _name;
	expToLevel = _skillExp;
}

Cleave::~Cleave() {
}

void Cleave::Activate(Unit* character, Unit* monster)
{
	if(character != nullptr && character->life >= 0)
	{
		DmgEnemy(monster, sliceDmg);
	}
}
