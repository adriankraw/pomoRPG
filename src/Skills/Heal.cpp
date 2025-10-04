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

	void activate(Unit* c, Unit* m) override;
private:
	
};

Heal::Heal() {
}

Heal::~Heal() {
}

void Heal::activate(Unit* c, [[maybe_unused]] Unit* m){
	if(c != nullptr)
	HealSelf(*c, 10);
}
