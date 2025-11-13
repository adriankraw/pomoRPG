#pragma once
#include "Skills.h"

class Ragnarok : public Skills{
public:
	Ragnarok(std::string, const int);
	Ragnarok(Ragnarok &&) = default;
	Ragnarok(const Ragnarok &) = default;
	Ragnarok &operator=(Ragnarok &&) = default;
	Ragnarok &operator=(const Ragnarok &) = default;
	~Ragnarok() override;

	void Activate(Unit* character, Unit* monster) override;
private:

	const int fireballDmg{53};
	int cost{5};	
};

Ragnarok::Ragnarok(std::string _name, const int _skillExp) {
	name = _name;
	expToLevel = _skillExp;
}

Ragnarok::~Ragnarok() {
}

void Ragnarok::Activate(Unit* character, Unit* monster){
	if(character!= nullptr)
	{
		DmgEnemy(monster, fireballDmg);
	}
}
