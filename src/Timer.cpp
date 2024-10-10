#pragma once
#include "Time.cpp"

enum TimerState{
	paused,
	countDown,
	countUp,
	ticking
};

class Timer {
public:
	Timer();
	Timer(TimerState state, Time startTime = Time(0,0,0,0));
	bool isRunning;
	bool isPaused;
	void Tick(const TimerState &state, Time &currentTime, double &deltaTime);
	void Tick(Time &currentTime, double &deltaTime);
	void SetState(TimerState);
	TimerState GetState();
	void SetTime(double,double,double,double);
	

private:
	Time starttime;
	Time time;
	TimerState currentState;
	void loopTimerTicks(float*);
};


Timer::Timer(){
	currentState = TimerState::countDown;
}
Timer::Timer(TimerState state, Time time) {
	currentState = state;
	starttime = time;
	isRunning = false;
	isPaused = false;
}
void loopTimerTicks(const float &currentTimer, const double &deltaTime)
{

}
void Timer::Tick(Time &currentTime, double &deltaTime){
	Timer::Tick(currentState, currentTime, deltaTime);
}
void Timer::Tick(const TimerState &state, Time &currentTime, double &deltaTime){
	switch(state)
	{
		case TimerState::countDown:
			if((currentTime.GetHour()+currentTime.GetMinute()+currentTime.GetSeconds()+currentTime.GetMili())-deltaTime < 0)
			{
				currentTime.resetTime();
				isRunning = false;
			}else{
				currentTime.subtractTime(currentTime, deltaTime);
			}
		break;
		case TimerState::countUp:
			currentTime.addTime(currentTime, deltaTime);
		break;
		default:
		break;
	}
}
void Timer::SetState(TimerState state)
{
	Timer::currentState = state;
}
TimerState Timer::GetState()
{
	return currentState;
}
void Timer::SetTime(double h, double m, double s, double mili)
{
	Timer::time.GetHour() = h;
	Timer::time.GetMinute() = m;
	Timer::time.GetSeconds() = s;
	Timer::time.GetMili() = mili;
	isRunning = true;
}
