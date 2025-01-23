#include "Timer.cpp"
#include <string>

class stopwatch{
public:
	stopwatch(std::string nameOfActivity, std::string nameOfSkill);
	stopwatch(std::string nameOfActivity, Time currentTime, std::string nameOfSkill);
	stopwatch(stopwatch &&) = default;
	stopwatch(const stopwatch &) = default;
	stopwatch &operator=(stopwatch &&) = default;
	stopwatch &operator=(const stopwatch &) = default;
	~stopwatch();

	Time* GetcurrentTime();
	std::string GetcurrentTimeAsString();
	Timer* GetTimer();
	std::string* GetName();

private:
	std::string nameOfActivity;
	std::string nameOfSkill;
	Timer timer;
	Time currentTime;	
};

stopwatch::stopwatch(std::string _nameOfActivity, std::string _nameOfSkill = "")
{
	stopwatch::nameOfActivity = _nameOfActivity;
	stopwatch::currentTime = Time();
	stopwatch::timer = Timer(TimerState::countUp, currentTime);
}

stopwatch::stopwatch(std::string _nameOfActivity, Time _currentTime, std::string _nameOfSkill = "")
{
	stopwatch::nameOfActivity = _nameOfActivity;
	stopwatch::currentTime = _currentTime;
	stopwatch::timer = Timer(TimerState::countUp, stopwatch::currentTime);
}

stopwatch::~stopwatch() {
}

Time* stopwatch::GetcurrentTime()
{
	return &currentTime;
}

std::string stopwatch::GetcurrentTimeAsString()
{
	std::string time;
	time.append(std::to_string(stopwatch::currentTime.GetHour()));
	time.append("h");
	time.append(std::to_string(stopwatch::currentTime.GetMinute()));
	time.append("m");
	time.append(std::to_string(stopwatch::currentTime.GetSeconds()));
	time.append("s");
	time.append(std::to_string(stopwatch::currentTime.GetMili()));
	time.append("mili");
	return time;
}
Timer* stopwatch::GetTimer()
{
	return &timer;
}
std::string* stopwatch::GetName() 
{
	return &(stopwatch::nameOfActivity);
}
