#include "Timer.cpp"
#include <string>

class stopwatch{
public:
	stopwatch(std::string nameOfActivity);
	stopwatch(std::string nameOfActivity, Time currentTimer);
	stopwatch(stopwatch &&) = default;
	stopwatch(const stopwatch &) = default;
	stopwatch &operator=(stopwatch &&) = default;
	stopwatch &operator=(const stopwatch &) = default;
	~stopwatch();

	Time* GetcurrentTime();
	std::string GetcurrentTimeAsString();
	Timer& GetTimer() {return stopwatch::timer;}
	std::string& GetName() {return stopwatch::nameOfActivity;}

private:
	std::string nameOfActivity;
	Timer timer;
	Time currentTimer;	
};

stopwatch::stopwatch(std::string _nameOfActivity)
{
	stopwatch::nameOfActivity = _nameOfActivity;
	stopwatch::currentTimer = Time();
	stopwatch::timer = Timer(TimerState::countUp, currentTimer);
}

stopwatch::stopwatch(std::string _nameOfActivity, Time _currentTimer)
{
	stopwatch::nameOfActivity = _nameOfActivity;
	stopwatch::currentTimer = _currentTimer;
	stopwatch::timer = Timer(TimerState::countUp, currentTimer);
}

stopwatch::~stopwatch() {
}

Time* stopwatch::GetcurrentTime()
{
	return &currentTimer;
}

std::string stopwatch::GetcurrentTimeAsString()
{
	std::string time;
	time.append(std::to_string(stopwatch::currentTimer.GetHour()));
	time.append("h");
	time.append(std::to_string(stopwatch::currentTimer.GetMinute()));
	time.append("m");
	time.append(std::to_string(stopwatch::currentTimer.GetSeconds()));
	time.append("s");
	time.append(std::to_string(stopwatch::currentTimer.GetMili()));
	time.append("mili");
	return time;
}
