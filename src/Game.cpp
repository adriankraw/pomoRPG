#pragma once

#include <vector>

#include "saveGame.cpp"
#include "Area.cpp"
#include "Character.cpp"
#include "Skilltree.cpp"

struct Event{
	bool running = false;
	Character::CharEvent eventType = Character::CharEvent::Nothing;
	std::shared_ptr<Monster> monster;
};

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
	Event currentEvent{};
	Timer globalTimer;
	Timer screenTimer;

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
