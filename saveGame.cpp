#include <fstream>
#include <iostream>
#include <string>
class saveGame{
public:
	saveGame();
	saveGame(saveGame &&) = default;
	saveGame(const saveGame &) = default;
	saveGame &operator=(saveGame &&) = default;
	saveGame &operator=(const saveGame &) = default;
	~saveGame();

	int lvl;

private:
	
};

saveGame::saveGame() {

	std::ifstream saveFile;
	saveFile.open("saveFile.txt");
	std::string line("");
	if(saveFile.is_open())
	{
		while (std::getline(saveFile, line))
		{
			if(line.find(":::") != 0)
			{
				std::string startingWith = line.substr(0, line.find(":::"));
				std::cout << startingWith << ":";
			}
			lvl = std::stoi(line.substr(line.find(":::")+3, line.length()));
			std::cout << lvl << std::endl;
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n').get();
		}
		saveFile.close();
	}
}

saveGame::~saveGame() {
}
