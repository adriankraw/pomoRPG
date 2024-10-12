#pragma once

#include "Rarity.cpp"
#include "Area.cpp"

class Game {
public:
	Game();
	Game(Game &&) = default;
	Game(const Game &) = default;
	Game &operator=(Game &&) = default;
	Game &operator=(const Game &) = default;
	~Game();

private:
	Area area;
	Rarity rarity;
	
};

Game::Game() {
	area = Area();
	rarity = Rarity();
}

Game::~Game() {
}
