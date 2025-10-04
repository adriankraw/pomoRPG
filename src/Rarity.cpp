#pragma once

class Rarity {
public:
	Rarity();
	Rarity(Rarity &&) = default;
	Rarity(const Rarity &) = default;
	Rarity &operator=(Rarity &&) = default;
	Rarity &operator=(const Rarity &) = default;
	~Rarity();

	enum Level
	{
		Common,
		Uncommon,
		Magic,
		Rare,
		Legendary
	};

	Level RollRarity(int);


private:

};

Rarity::Rarity() {
}

Rarity::~Rarity() {
}

Rarity::Level Rarity::RollRarity(int areaId)
{
	if(areaId >= 0)
	{
		return Rarity::Level::Common;
	}else{
		return Rarity::Level::Uncommon;
	}
}
