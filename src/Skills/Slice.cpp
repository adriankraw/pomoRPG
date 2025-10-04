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

	void activate(Unit* c, Unit* m) override;

private:
	
};

Slice::Slice() {
}

Slice::~Slice() {
}

void Slice::activate(Unit* c, Unit* m)
{
	if(c != nullptr && c->life >= 0)
		DmgEnemy(*m, 15);
}
