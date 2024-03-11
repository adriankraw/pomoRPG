#include <chrono>
#include <cstdio>
#include <ios>
#include <iostream>
#include <limits>
#include <ostream>
#include <ratio>
#include <thread>
#include <stdio.h>

void countingTimer(double currentTimer)
{	
	while(currentTimer > 0)
	{
		auto startFrame = std::chrono::system_clock::now();

		printf( "countdown: %i \n \e[1;1H\e[2J", (int)currentTimer/1000);



		std::this_thread::sleep_for(std::chrono::milliseconds(1000/15));
		auto endFrame = std::chrono::system_clock::now();
		double deltaTime = std::chrono::duration<double, std::milli>(endFrame-startFrame).count();
		if(currentTimer-deltaTime < 0)
		{
			currentTimer = 0;
		}else{
			currentTimer -= deltaTime;
		}
	}

}

int main (int argc, char *argv[]) {
	std::cout << "\e[1;1H\e[2J" <<"Starting PomoRPG... \n";

	double worktimer(10);
	double braketimer(2);
	double countdown(10000);
	bool running(true);

	std::cout << "welcome to your own liddle pomodoro timer \n \n";
	std::cout << "how long do you want to work ? (minutes)";
	std::cin >> worktimer;
	std::cout << "how long do you want your brake to be? (minutes)";
	std::cin >> braketimer;
	std::cout << "\e[1;1H\e[2J";
	
	while(countdown > 0)
	{
		auto startFrame = std::chrono::system_clock::now();
		std::cout << (int)(countdown/1000) << "\n";

		std::this_thread::sleep_for(std::chrono::milliseconds(1000/15));
		auto endFrame = std::chrono::system_clock::now();
		double deltaTime = std::chrono::duration<double, std::milli>(endFrame-startFrame).count();
		countdown -= deltaTime;
		std::cout << "\e[1;1H\e[2J";
	}
	std::cout << "To Start Working press any key...";
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n').get();

	while(running)
	{
		countingTimer(worktimer * 1000 * 60);
		//	notify
		//	breaketime
		std::cout << "Press Enter key to continue...";
		std::cin.get();
		
		countingTimer(braketimer * 1000 * 60);
		//	notify
		//	wait for user imput
		std::cout << "Press Enter key to continue...";
		std::cin.get();
	}


	return 0;
}
