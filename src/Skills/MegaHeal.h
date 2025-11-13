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

	void Activate(Unit* character, Unit* monster) override;
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

void MegaHeal::Activate(Unit* character, [[maybe_unused]] Unit* monster){
	if(character != nullptr)
	{
		HealSelf(character, healPower);
	}
}
