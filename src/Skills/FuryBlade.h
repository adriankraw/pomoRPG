#pragma once
#include "Skills.h"

class FuryBlade: public Skills {
public:
	FuryBlade(std::string, const int);
	FuryBlade(FuryBlade &&) = default;
	FuryBlade(const FuryBlade &) = default;
	FuryBlade &operator=(FuryBlade &&) = default;
	FuryBlade &operator=(const FuryBlade &) = default;
	~FuryBlade() override;

	void Activate(Unit& character, std::shared_ptr<Unit> monster) override;

private:

	const int sliceDmg{34};
	int cost{3};	
};

FuryBlade::FuryBlade(std::string _name, const int _skillExp) {
	name = _name;
	expToLevel = _skillExp;
}

FuryBlade::~FuryBlade() {
}

void FuryBlade::Activate(Unit& character, std::shared_ptr<Unit> monster)
{
	if(character.life >= 0)
	{
		DmgEnemy(monster, sliceDmg);
	}
}
