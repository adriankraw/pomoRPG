#include "Timer.cpp"
#include <string>

class stopwatch{
public:
	stopwatch(std::string nameOfActivity, double currentTimer);
	stopwatch(stopwatch &&) = default;
	stopwatch(const stopwatch &) = default;
	stopwatch &operator=(stopwatch &&) = default;
	stopwatch &operator=(const stopwatch &) = default;
	~stopwatch();

	double& GetcurrentTime();
	Timer& GetTimer() {return stopwatch::timer;}
	std::string& GetName() {return stopwatch::nameOfActivity;}

private:
	std::string nameOfActivity;
	Timer timer;
	double currentTimer;	
};

stopwatch::stopwatch(std::string _nameOfActivity, double _currentTimer)
{
	stopwatch::nameOfActivity = _nameOfActivity;
	stopwatch::currentTimer = _currentTimer;
	stopwatch::timer = Timer(TimerState::countUp, currentTimer);
}

stopwatch::~stopwatch() {
}

double& stopwatch::GetcurrentTime()
{
	return stopwatch::currentTimer;
}
