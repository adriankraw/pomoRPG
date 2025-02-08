#include "Timer.cpp"
#include <string>

class stopwatch{
public:
<<<<<<< HEAD
	stopwatch(std::string nameOfActivity);
	stopwatch(std::string nameOfActivity, Time currentTime);
=======
	stopwatch(std::string nameOfActivity, std::string nameOfSkill);
	stopwatch(std::string nameOfActivity, Time currentTime, std::string nameOfSkill);
>>>>>>> skills
	stopwatch(stopwatch &&) = default;
	stopwatch(const stopwatch &) = default;
	stopwatch &operator=(stopwatch &&) = default;
	stopwatch &operator=(const stopwatch &) = default;
	~stopwatch();

	Time* GetcurrentTime();
	std::string GetcurrentTimeAsString();
	Timer* GetTimer();
	std::string* GetName();
<<<<<<< HEAD

private:
	std::string nameOfActivity;
=======
	std::string& GetNameOfCorrespondingSkill();

private:
	std::string nameOfActivity;
	std::string nameOfSkill;
>>>>>>> skills
	Timer timer;
	Time currentTime;	
};

<<<<<<< HEAD
stopwatch::stopwatch(std::string _nameOfActivity)
=======
stopwatch::stopwatch(std::string _nameOfActivity, std::string _nameOfSkill = "")
>>>>>>> skills
{
	stopwatch::nameOfActivity = _nameOfActivity;
	stopwatch::currentTime = Time();
	stopwatch::timer = Timer(TimerState::countUp, currentTime);
<<<<<<< HEAD
}

stopwatch::stopwatch(std::string _nameOfActivity, Time _currentTime)
=======
	stopwatch::nameOfSkill = _nameOfSkill;
}

stopwatch::stopwatch(std::string _nameOfActivity, Time _currentTime, std::string _nameOfSkill = "")
>>>>>>> skills
{
	stopwatch::nameOfActivity = _nameOfActivity;
	stopwatch::currentTime = _currentTime;
	stopwatch::timer = Timer(TimerState::countUp, stopwatch::currentTime);
<<<<<<< HEAD
=======
	stopwatch::nameOfSkill = _nameOfSkill;
>>>>>>> skills
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
<<<<<<< HEAD
	return &(stopwatch::nameOfActivity);
=======
	return &(nameOfActivity);
}
std::string& stopwatch::GetNameOfCorrespondingSkill() 
{
	return nameOfSkill;
>>>>>>> skills
}
