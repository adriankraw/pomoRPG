#pragma once

<<<<<<< HEAD
#include "Area.cpp"
=======
#include <vector>

#include "saveGame.cpp"
#include "Area.cpp"
#include "Character.cpp"
#include "Skills.cpp"
#include "Skilltree.cpp"
>>>>>>> skills

class Game {
public:
	Game();
	Game(Game &&) = default;
	Game(const Game &) = default;
	Game &operator=(Game &&) = default;
	Game &operator=(const Game &) = default;
	~Game();
<<<<<<< HEAD
=======
	
	saveGame save;
	Character player;
	Skilltree skilltree;
	std::vector<Skills> playerSkills;

	void setSkilltree();
>>>>>>> skills

private:
	Area area;
	Rarity rarity;
<<<<<<< HEAD
	
=======
>>>>>>> skills
};

Game::Game() {
	area = Area();
}

Game::~Game() {
}
<<<<<<< HEAD
=======

void Game::setSkilltree()
{

}
>>>>>>> skills
