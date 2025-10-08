#pragma once

#include <cstdint>
#include <map>
#include <string>

class Commands {
public:
	enum Base:std::uint8_t{
		time,
		CountUp,
		CountDown,
		PrintAll,
		up,
		down,
		save,
		timer,
		add,
		pause,
		stop,
		start,
		resume,
		bigclock,
		charsettings,
		charstats,
		stopwatches,
		fight,
		eventlist,
		circle,
		exit,
		help
	};

	using _commandsMap = std::map<Base, std::string>;
	static _commandsMap commandsMap;
private:
	
};

Commands::_commandsMap Commands::commandsMap = {
	{Base::time, "time"},
	{Base::start, "start"},
	{Base::pause, "pause"},
	{Base::add, "add"},
	{Base::CountUp, "countup"},
	{Base::CountDown, "countdown"},
	{Base::PrintAll, "printall"},
	{Base::up, "up"},
	{Base::down, "down"},
	{Base::save, "save"},
	{Base::timer, "timer"},
	{Base::add, "add"},
	{Base::pause, "pause"},
	{Base::stop, "stop"},
	{Base::start, "start"},
	{Base::resume, "resume"},
	{Base::bigclock, "bigclock"},
	{Base::charsettings, "charsettings"},
	{Base::charstats, "charstats"},
	{Base::stopwatches, "stopwatches"},
	{Base::fight, "fight"},
	{Base::eventlist, "eventlist"},
	{Base::circle, "circle"},
	{Base::exit, "exit"},
	{Base::help, "help"},
};

//std::vector<std::string> ARGV = {"-countUp","-countDown","-time","-start"};
