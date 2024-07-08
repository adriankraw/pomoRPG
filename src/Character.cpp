#include <cmath>
#include <functional>
#include <regex>
#include <string>
#include <vector>

class Character{
public:
	Character();
	Character(std::string, int, int);
	Character(Character &&) = default;
	Character(const Character &) = default;
	Character &operator=(Character &&) = default;
	Character &operator=(const Character &) = default;


	void SetName(const std::string&);
	void SetLvl(const int&);
	void SetExp(const int&);

	const std::string Name(){return Character::name;}
	const int Lvl(){return Character::lvl;}
	const int Exp(){return Character::exp;}

	void levelUp();
	int GetNextLevelExp();
	void addLevelUpAction(void(*)());

private:
	std::string name;
	int lvl;
	int exp;
	std::vector<void (*)()> levelupActions;
};
Character::Character(){};

Character::Character(const std::string _name, const int _lvl, const int _exp):
	name(_name), 
	lvl(_lvl),
	exp(_exp){
}

void Character::SetName(const std::string &_name) {
	Character::name = _name;
}
void Character::SetLvl(const int &_lvl) {
	Character::lvl = _lvl;
}
void Character::SetExp(const int &_exp) {
	if(_exp >= GetNextLevelExp()){
		Character::exp = _exp%GetNextLevelExp();
		Character::levelUp();
	}else{
		Character::exp = _exp;
	}
}

void Character::levelUp(){
	++lvl;
	for (auto func: Character::levelupActions) {
		func();
	}
	//save the char
}
void Character::addLevelUpAction(void (*action)())
{
	Character::levelupActions.push_back(action);
}
int Character::GetNextLevelExp(){
	return std::pow(std::pow(Character::lvl-1,2)+4,2);
}
