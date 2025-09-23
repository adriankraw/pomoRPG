#pragma once

#include <vector>

#include "saveGame.cpp"
#include "Area.cpp"
#include "Character.cpp"
#include "Skilltree.cpp"

class Game {
public:
	Game();
	Game(Game &&) = default;
	Game(const Game &) = default;
	Game &operator=(Game &&) = default;
	Game &operator=(const Game &) = default;
	~Game();
	
	saveGame save;
	Character player;
	Skilltree skilltree;

	void setSkilltree();

private:
	Area area;
	Rarity rarity;
};

Game::Game() {
	area = Area();
}

Game::~Game() {
}

void Game::setSkilltree()
{

}
