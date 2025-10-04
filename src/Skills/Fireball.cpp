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

	void activate(Unit* c, Unit* m) override;
private:
	
};

Fireball::Fireball() {
}

Fireball::~Fireball() {
}

void Fireball::activate(Unit* c, Unit* m){
	if(c!= nullptr)
	DmgEnemy(*m, 10);
}
