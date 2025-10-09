#pragma once
#include "./Skills.cpp"

class Slice: public Skills {
public:
	Slice();
	Slice(Slice &&) = default;
	Slice(const Slice &) = default;
	Slice &operator=(Slice &&) = default;
	Slice &operator=(const Slice &) = default;
	~Slice() override;

	void Activate(Unit* character, Unit* monster) override;

private:

	const int sliceDmg{15};
	
};

Slice::Slice() {
}

Slice::~Slice() {
}

void Slice::Activate(Unit* character, Unit* monster)
{
	if(character != nullptr && character->life >= 0)
	{
		DmgEnemy(monster, sliceDmg);
	}
}
