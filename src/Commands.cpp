#include <map>
#include <string>
class Commands {
public:
	enum Base{
		time,
		start,
		pause,
		add,
		CountUp,
		CountDown,
	};

	typedef std::map<Base, std::string> _commandsMap;
	static _commandsMap CommandsMap;
private:
	
};

Commands::_commandsMap Commands::CommandsMap = {
	{Base::time, "-time"},
	{Base::start, "-start"},
	{Base::pause, "-pause"},
	{Base::add, "-add"},
	{Base::CountUp, "-countup"},
	{Base::CountDown, "-countdown"},
};

//std::vector<std::string> ARGV = {"-countUp","-countDown","-time","-start"};
