#pragma once
#include "Skills.h"

class Heal : public Skills{
public:
	Heal(std::string, const int);
	Heal(Heal &&) = default;
	Heal(const Heal &) = default;
	Heal &operator=(Heal &&) = default;
	Heal &operator=(const Heal &) = default;
	~Heal() override;

	void Activate(Unit* character, Unit* monster) override;
private:
	const int healPower{10};
	int cost{2};
};

Heal::Heal(std::string _name, const int _skillExp){
	name = _name;
	expToLevel = _skillExp;
}		

Heal::~Heal() {
}

void Heal::Activate(Unit* character, [[maybe_unused]] Unit* monster){
	if(character != nullptr)
	{
		HealSelf(character, healPower);
	}
}
