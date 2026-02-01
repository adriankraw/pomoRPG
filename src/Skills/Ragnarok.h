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

	void Activate(Unit& character, std::shared_ptr<Unit> monster) override;
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

void Ragnarok::Activate(Unit& character, std::shared_ptr<Unit> monster){
	if(character.life>0)
	{
		DmgEnemy(monster, fireballDmg);
	}
}
