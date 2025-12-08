#pragma once

#include <map>
#include <typeindex>
#include "./Skills/Skills.h"
#include "./Skills/Heal.h"
#include "./Skills/Slice.h"
#include "./Skills/Fireball.h"

struct SkillType{
std::type_index index;
std::function<Skills*(std::string, const int)> contructor;
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
	{"Heal",	{ typeid(Heal),		[](std::string name, const int skillExp) -> Skills* { return new Heal(name, skillExp); } }},
	{"Slice",	{ typeid(Slice),	[](std::string name, const int skillExp) -> Skills* { return new Slice(name, skillExp); } }},
	{"Fireball",	{ typeid(Fireball),	[](std::string name, const int skillExp) -> Skills* { return new Fireball(name, skillExp); } }},
	};
}

Skilltree::~Skilltree() {
}
