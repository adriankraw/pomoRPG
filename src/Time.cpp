#pragma once

class Time {
public:
	Time();
	Time(int,int,int,int);
	Time(Time &&) = default;
	Time(const Time &) = default;
	Time &operator=(Time &&) = default;
	Time &operator=(const Time &) = default;
	~Time();

	int& GetHour() {return hour;}
	int& GetMinute() {return minute;}
	int& GetSeconds() {return second;}
	int& GetMili() {return mili;}

	void SetHour(int value) { hour = value;}
	void SetMinute(int value) { minute = value;}
	void SetSeconds(int value) { second = value;}
	void SetMili(int value) { mili = value;}

	int ConvertToSecondsForModulo(int);

	void addTime(Time&, const double);
	void subtractTime(Time&, const double);
	void resetTime();

private:
	int hour;
	int minute;
	int second;
	int mili;
};

Time::Time() {
	hour = 0;
	minute = 0;
	second = 0;
	mili = 0;
}
Time::Time(int _hour, int _minute, int _second, int _mili)
{
	hour = _hour;
	minute = _minute;
	second = _second;
	mili = _mili;
}

Time::~Time() {
}

int Time::ConvertToSecondsForModulo(int maxCount)
{
	int sec = second;
	if(maxCount < sec)
	{
		sec = sec%maxCount;
	}else{
		if(minute > 0)
		{
			sec = (sec+60*minute)%maxCount;
		}else if (hour > 0)
		{
			sec = (sec+60*60)%maxCount;
		}
	}
	return sec; 
}

void Time::addTime(Time& time, const double timeToAdd)
{
	time.GetMili() += timeToAdd;
	while(time.GetMili()>1000)
	{
		time.GetSeconds()+=1;
		time.GetMili()-= 1000;
	}
	while(time.GetSeconds()>60)
	{
		time.GetMinute()+= 1;
		time.GetSeconds()-=60;
	}
	while(time.GetMinute()>60)
	{
		time.GetHour()+=1;
		time.GetMinute()-=60;
	}
}

void Time::subtractTime(Time& time, const double timeToSubtract)
{
	while(time.GetMili()<timeToSubtract)
	{
		time.GetSeconds()-=1;
		time.GetMili()+=1000;
	}
	time.GetMili()-=timeToSubtract;
}


void Time::resetTime()
{
	hour = 0;
	minute = 0;
	second = 0;
	mili = 0;
}
