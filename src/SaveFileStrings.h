#include <string>

class SaveFileStrings{
	private:
	std::string StringList[3] = {"Spacer", "stopped", "started"};

	public:
	enum SaveFileKey{
		SPACER = 0,
		STOPPED = 1,
		STARTED = 2,
	}; 
	std::string SaveFileValue(int key){return StringList[key];}
};
