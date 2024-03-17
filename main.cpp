#include <chrono>
#include <cstdio>
#include <iomanip>
#include <ios>
#include <iostream>
#include <limits>
#include <locale>
#include <ostream>
#include <ratio>
#include <string>
#include <thread>
#include <stdio.h>


#include "render.cpp"

std::chrono::time_point<std::chrono::system_clock> startFrame;
std::chrono::time_point<std::chrono::system_clock> endFrame;
double deltaTime(0);
render ren;

void countingTimer(double currentTimer)
{	
	while(currentTimer > 0)
	{
		std::cout << "\e[1H" << std::flush;
		startFrame = std::chrono::system_clock::now();
		
		std::cout << std::left << "countdown: \e[1m"<< std::setfill('.') << std::setw(50) << std::right << currentTimer/1000 << "\e[0m";


		std::this_thread::sleep_for(std::chrono::seconds(1));
		
		endFrame = std::chrono::system_clock::now();
		deltaTime = std::chrono::duration<double, std::milli>(endFrame-startFrame).count();

		if(currentTimer-deltaTime < 0)
		{
			currentTimer = 0;
		}else{
			currentTimer -= deltaTime;
		}
	}

}

int main (int argc, char *argv[]) {
	std::cout << "\e[2J \e[1H" <<"Starting PomoRPG... \n";

	double worktimer(10);
	double braketimer(2);
	double countdown(10000);
	bool running(true);

	std::cout << "welcome to your own liddle pomodoro timer \n \n";
	std::cout << "how long do you want to work ? (minutes)";
	for(int i = 0; i < 10; ++i)
	{
		std::cout << std::endl;
		for(std::string s: *ren.renderTime(i))
		{
			std::cout << s <<std::endl;
		}
	}
	std::cin >> worktimer;
	worktimer*= 60000;
	std::cout << "how long do you want your brake to be? (minutes)";
	std::cin >> braketimer;
	braketimer *= 60000;

	std::cout << "To Start Working press any key...";
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n').get();

	std::cout << "\e[2J";
	while(running)
	{
		countingTimer(worktimer);
		//	notify
		//	breaketime
		std::cout << "\e[1J \e[1H" << "Press Enter key to continue...";
		std::cin.get();
		
		countingTimer(braketimer);
		//	notify
		//	wait for user imput
		std::cout << "\e[1J \e[1H" << "Press Enter key to continue...";
		std::cin.get();
	}


	return 0;
}
