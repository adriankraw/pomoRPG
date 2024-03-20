#include <array>
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
		std::cout << "\033[1J \033[1H" << std::flush;
		startFrame = std::chrono::system_clock::now();
		
		std::cout << std::left << "countdown: "<< std::endl;
		ren.renderTime(currentTimer);
		std::cout << "\033[1m";
		for(int i(0); i < (*ren.resultpointer).size(); ++i)
		{
			std::cout << (*ren.resultpointer)[i] <<std::endl;
		}
		std::cout << "\033[0m";

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
	std::cout << "\033[2J \033[1H" <<"Starting PomoRPG... \n";

	double worktimer(10);
	double braketimer(2);
	double countdown(10000);
	bool running(true);

	std::cout << "welcome to your own liddle pomodoro timer \n \n";
	std::cout << "how long do you want to work ? (minutes)";
	std::cin >> worktimer;


	worktimer*= 60000;
	std::cout << "how long do you want your brake to be? (minutes)";
	std::cin >> braketimer;
	braketimer *= 60000;

	std::cout << "To Start Working press any key...";
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n').get();

	std::cout << "\033[2J";
	while(running)
	{
		countingTimer(worktimer);
		//	notify
		//	breaketime
		std::cout << "\033[1J \033[1H" << "Press Enter key to continue...";
		std::cin.get();
		
		countingTimer(braketimer);
		//	notify
		//	wait for user imput
		std::cout << "\033[1J \033[1H" << "Press Enter key to continue...";
		std::cin.get();
	}


	return 0;
}
