#pragma once

#include <vector>
#include "Skills.cpp"

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
