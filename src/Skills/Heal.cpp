#pragma once
#include "Skills.cpp"
#include <cstddef>

class Heal : public Skills{
public:
	Heal();
	Heal(Heal &&) = default;
	Heal(const Heal &) = default;
	Heal &operator=(Heal &&) = default;
	Heal &operator=(const Heal &) = default;
	~Heal() override;

	void Activate(Unit* character, Unit* monster) override;
private:

	const int healPower{10};
	
};

Heal::Heal() {
}

Heal::~Heal() {
}

void Heal::Activate(Unit* character, [[maybe_unused]] Unit* monster){
	if(character != nullptr)
	{
		HealSelf(character, healPower);
	}
}
