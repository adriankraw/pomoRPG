#pragma once
#include "Skills.h"

class Slice: public Skills {
public:
	Slice(std::string, const int);
	Slice(Slice &&) = default;
	Slice(const Slice &) = default;
	Slice &operator=(Slice &&) = default;
	Slice &operator=(const Slice &) = default;
	~Slice() override;

	void Activate(Unit* character, Unit* monster) override;

private:

	const int sliceDmg{15};
};

Slice::Slice(std::string _name, const int _skillExp) {
	name = _name;
	expToLevel = _skillExp;
}

Slice::~Slice() {
}

void Slice::Activate(Unit* character, Unit* monster)
{
	if(character != nullptr && character->life >= 0)
	{
		DmgEnemy(monster, sliceDmg);
	}
}
