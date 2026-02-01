#pragma once
#include "Skills.h"

class Thunder : public Skills{
public:
	Thunder(std::string, const int);
	Thunder(Thunder &&) = default;
	Thunder(const Thunder &) = default;
	Thunder &operator=(Thunder &&) = default;
	Thunder &operator=(const Thunder &) = default;
	~Thunder() override;

	void Activate(Unit& character, std::shared_ptr<Unit> monster) override;
private:

	const int fireballDmg{17};
	int cost{2};
};

Thunder::Thunder(std::string _name, const int _skillExp) {
	name = _name;
	expToLevel = _skillExp;
}

Thunder::~Thunder() {
}

void Thunder::Activate(Unit& character, std::shared_ptr<Unit> monster){
	if(character.life>0)
	{
		DmgEnemy(monster, fireballDmg);
	}
}
