#include <chrono>
#include <cstdio>
#include <iostream>
#include <limits>
#include <ratio>
#include <thread>
#include <stdio.h>

void countingTimer(double currentTimer)
{	
	while(currentTimer > 0)
	{
		auto startFrame = std::chrono::system_clock::now();

		printf( "countdown: %i \n \ec", (int)currentTimer/1000);



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
	printf("\ec");
	printf("Starting PomoRPG... \n");

	double worktimer(10);
	double braketimer(2);
	double countdown(10000);

	printf("welcome to your own liddle pomodoro timer \n:\n");
	printf("how long do you want to work ? (minutes)");
	scanf("%lf", &worktimer);
	printf("how long do you want your brake to be? (minutes)");
	scanf("%lf", &braketimer);
	
	while(countdown > 0)
	{
		auto startFrame = std::chrono::system_clock::now();
		std::cout << "starting in:" << (int)(countdown/1000) << "%i \n \ec";

		std::this_thread::sleep_for(std::chrono::milliseconds(1000/15));
		auto endFrame = std::chrono::system_clock::now();
		double deltaTime = std::chrono::duration<double, std::milli>(endFrame-startFrame).count();
		countdown -= deltaTime;
	}
	std::cout << "To Start Working press any key" << std::cin.get();

	bool running(true);
	while(running)
	{
		countingTimer(worktimer * 1000 * 60);
		//	notify
		//	breaketime
		printf("Press Enter key to continue...");
		std::cin.get();
		
		countingTimer(braketimer * 1000 * 60);
		//	notify
		//	wait for user imput
		printf("Press Enter key to continue...");
		std::cin.get();
	}


	return 0;
}
