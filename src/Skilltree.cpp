#pragma once

#include <map>
#include <typeindex>
#include "./Skills/Skills.cpp"
#include "./Skills/Heal.cpp"
#include "./Skills/Slice.cpp"
#include "./Skills/Fireball.cpp"

struct SkillType{
std::type_index index;
std::function<Skills*()> contructor;
};

class Skilltree {
public:
	Skilltree();
	~Skilltree();
	std::map<std::string, SkillType> skillvector;

private:

};

Skilltree::Skilltree() {
	skillvector = {
	{"Heal",	{ typeid(Heal),		[]() -> Skills* { return new Heal(); } }},
	{"Slice",	{ typeid(Slice),	[]() -> Skills* { return new Slice(); } }},
	{"Fireball",	{ typeid(Fireball),	[]() -> Skills* { return new Fireball(); } }},
	};
}

Skilltree::~Skilltree() {
}
