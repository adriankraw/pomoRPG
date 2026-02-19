#pragma once

#include <vector>

#include "Monster.cpp"
#include "Rarity.cpp"

class Area {
public:
	struct AreaProps{
		std::string name;
		int level;
		int monsterLevel;
	};
	Area();
	Area(const AreaProps&);
	Area(Area &&) = default;
	Area(const Area &) = default;
	Area &operator=(Area &&) = default;
	Area &operator=(const Area &) = default;
	~Area();

	std::string* GetName();
	Monster* Getmonster();
	Rarity::Level GetRandomRarety();

	void RollItem(Rarity::Level* rarity, int &itemCode, int &itemAmount);

private:
	std::string name;
	int areaLevel = 0;
	Rarity rarity;
	std::vector<Monster> monsterList;
};

Area::Area() {
	name = "Office";
	areaLevel = 0;
	monsterList = {Monster(1000)};
	Area::rarity = Rarity();
}
Area::Area(const Area::AreaProps& props) {
	name = props.name;
	areaLevel = props.level;
	monsterList = {Monster(props.monsterLevel * 1000)};
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
	return &monsterList.front();
}

Rarity::Level Area::GetRandomRarety()
{
	return (rarity.RollRarity(areaLevel));
}

void Area::RollItem(Rarity::Level* rarity, int &itemCode, int &itemAmount )
{
	if(rarity != nullptr)
	{
		itemCode = itemCode;
		itemAmount = itemAmount;
	}else{
		itemCode = 0;
		itemAmount = 0;
	}
}
