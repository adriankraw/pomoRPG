#pragma once
#include "Skills.cpp"

class Fireball : public Skills{
public:
	Fireball();
	Fireball(Fireball &&) = default;
	Fireball(const Fireball &) = default;
	Fireball &operator=(Fireball &&) = default;
	Fireball &operator=(const Fireball &) = default;
	~Fireball() override;

	void Activate(Unit* character, Unit* monster) override;
private:

	const int fireballDmg{10};
	
};

Fireball::Fireball() {
}

Fireball::~Fireball() {
}

void Fireball::Activate(Unit* character, Unit* monster){
	if(character!= nullptr)
	{
		DmgEnemy(monster, fireballDmg);
	}
}
