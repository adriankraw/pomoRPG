#pragma once
#include "Skills.h"

class MegaHeal : public Skills{
public:
	MegaHeal(std::string, const int);
	MegaHeal(MegaHeal &&) = default;
	MegaHeal(const MegaHeal &) = default;
	MegaHeal &operator=(MegaHeal &&) = default;
	MegaHeal &operator=(const MegaHeal &) = default;
	~MegaHeal() override;

	void Activate(Unit& character, std::shared_ptr<Unit> monster) override;
private:

	const int healPower{45};
	int cost{4};	
};

MegaHeal::MegaHeal(std::string _name, const int _skillExp) {
	name = _name;
	expToLevel = _skillExp;
}

MegaHeal::~MegaHeal() {
}

void MegaHeal::Activate(Unit& character, [[maybe_unused]] std::shared_ptr<Unit> monster){
	if(character.life>0)
	{
		HealSelf(character, healPower);
	}
}
