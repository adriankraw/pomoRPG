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

	void Activate(Unit& character, std::shared_ptr<Unit> monster) override;
private:
	const int healPower{10};
};

Heal::Heal(std::string _name, const int _skillExp){
	name = _name;
	expToLevel = _skillExp;
	cost = 10;
}		

Heal::~Heal() {
}

void Heal::Activate(Unit& character, [[maybe_unused]] std::shared_ptr<Unit> monster){
	animationType = Char::AnimationType::HEAL;
	if(character.life > 0)
	{
		HealSelf(character, healPower);
	}
}
