#pragma once

class ItemDrop {
public:
	ItemDrop(){};
	ItemDrop(ItemDrop &&) = default;
	ItemDrop(const ItemDrop &) = default;
	ItemDrop &operator=(ItemDrop &&) = default;
	ItemDrop &operator=(const ItemDrop &) = default;
	~ItemDrop(){};

	int itemCode{0};
	int itemAmount{0};
	ItemDrop(int code,int amount):
	itemCode{code}, itemAmount{amount}
	{}
};
