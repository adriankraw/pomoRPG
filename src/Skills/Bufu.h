#pragma once
#include "Skills.h"

class Bufu : public Skills{
public:
	Bufu(std::string, const int);
	Bufu(Bufu &&) = default;
	Bufu(const Bufu &) = default;
	Bufu &operator=(Bufu &&) = default;
	Bufu &operator=(const Bufu &) = default;
	~Bufu() override;

	void Activate(Unit* character, Unit* monster) override;
private:

	const int fireballDmg{20};
	int cost{2};
	
};

Bufu::Bufu(std::string _name, const int _skillExp) {
	name = _name;
	expToLevel = _skillExp;
}

Bufu::~Bufu() {
}

void Bufu::Activate(Unit* character, Unit* monster){
	if(character!= nullptr)
	{
		DmgEnemy(monster, fireballDmg);
	}
}
