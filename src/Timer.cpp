#pragma once

enum TimerState{
	paused,
	countDown,
	countUp,
	ticking
};

class Timer {
public:
	Timer();
	Timer(TimerState state, float startTime = 0.0f);
	bool isRunning;
	bool isPaused;
	void Tick(const TimerState &state, double &currentTime, double &deltaTime);
	void Tick(double &currentTime, double &deltaTime);
	void SetState(TimerState);
	TimerState GetState();
	void SetTime(float);
	

private:
	float starttime;
	float time;
	TimerState currentState;
	void loopTimerTicks(float*);
};


Timer::Timer(){
	currentState = TimerState::countDown;
}
Timer::Timer(TimerState state, float time) {
	currentState = state;
	starttime = time;
	isRunning = false;
	isPaused = false;
}
void loopTimerTicks(const float &currentTimer, const double &deltaTime)
{

}
void Timer::Tick(double &currentTime, double &deltaTime){
	Timer::Tick(currentState, currentTime, deltaTime);
}
void Timer::Tick(const TimerState &state, double &currentTime, double &deltaTime){
	switch(state)
	{
		case TimerState::countDown:
			if(currentTime-deltaTime < 0)
			{
				currentTime= 0;
				isRunning = false;
			}else{
				currentTime -= deltaTime;
			}
		break;
		case TimerState::countUp:
			currentTime += deltaTime;
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
void Timer::SetTime(float time)
{
	Timer::time = time;
	isRunning = true;
}
