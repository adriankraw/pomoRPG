#include <cmath>
#include <regex>
#include <string>

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

private:
	std::string name;
	int lvl;
	int exp;
};
Character::Character(){};

Character::Character(const std::string _name, const int _lvl, const int _exp):
	name(_name), 
	lvl(_lvl), 
	exp(_exp){}

void Character::SetName(const std::string &_name) {
	Character::name = _name;
}
void Character::SetLvl(const int &_lvl) {
	Character::lvl = _lvl;
}
void Character::SetExp(const int &_exp) {
	if(_exp >= (std::pow(2,(3+Character::lvl)))){
		Character::exp = (_exp%(int)std::pow(2,(3+Character::lvl)));
		Character::levelUp();
	}else{
		Character::exp = _exp;
	}
}

void Character::levelUp(){
	++lvl;
	//save the char
}
