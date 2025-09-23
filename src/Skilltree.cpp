#pragma once

#include <vector>
#include "./Skills/Skills.cpp"

class Skilltree {
public:
	Skilltree();
	~Skilltree();
	std::vector<Skills> Skillvector;

private:
	
};

Skilltree::Skilltree() {
	Skillvector = std::vector<Skills>();
}

Skilltree::~Skilltree() {
}
