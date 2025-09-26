#pragma once

#include <vector>

#include "Monster.cpp"
#include "Rarity.cpp"

class Area {
public:
	Area();
	Area(Area &&) = default;
	Area(const Area &) = default;
	Area &operator=(Area &&) = default;
	Area &operator=(const Area &) = default;
	~Area();

	std::string* GetName();
	Monster* Getmonster();
	Rarity::Level GetRandomRarety();

	void RollItem(Rarity::Level*, int&, int&);

private:
	std::string name;
	int areaLevel = 0;
	Rarity rarity;
	std::vector<Monster> monsterList;
};

Area::Area() {
	monsterList.push_back(Monster(1000));
	areaLevel = 0;
	Area::rarity = Rarity();
}

Area::~Area() {
}

std::string* Area::GetName()
{
	return &name;
}

Monster* Area::Getmonster()
{
	return new Monster(monsterList[0]);
}

Rarity::Level Area::GetRandomRarety()
{
	return (rarity.RollRarity(areaLevel));
}

void Area::RollItem(Rarity::Level* rarity, int &itemCode, int &itemAmount )
{
	itemCode = 0;
	itemAmount = 1;
}
