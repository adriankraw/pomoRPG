#pragma once
class Unit {
public:
	int life{0};
	virtual void GetAttacked(int value) 
	{
		life -= value;
	}
	virtual void GetLife(int value) 
	{
		life += value;
	}
	virtual ~Unit(){};
};
