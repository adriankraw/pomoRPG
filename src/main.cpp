#include <algorithm>
#include <array>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <future>
#include <iomanip>
#include <ios>
#include <iostream>
#include <limits>
#include <locale>
#include <memory>
#include <new>
#include <ostream>
#include <ratio>
#include <string>
#include <thread>
#include <stdio.h>
#include <vector>

#define frames 60

#include "saveGame.cpp"
#include "Timer.cpp"
#include "printer.cpp"

std::chrono::time_point<std::chrono::system_clock> startFrame;
std::chrono::time_point<std::chrono::system_clock> endFrame;
double deltaTime(0);

std::vector<std::string> ARGV = {"-countUp","-countDown","-time"};

std::shared_ptr<std::string> keyboardInput = std::make_shared<std::string>();

void sleepfuntion(std::shared_ptr<std::string> cinText)
{
	std::cin >> *cinText;
}
void countingTimer(double &currentTimer, Timer *timer, saveGame *save, printer &print)
{
	double exp = 0;
	double animationTimer = 0;
	//ask for input
	std::thread thread_obj(&sleepfuntion, keyboardInput);
	thread_obj.detach();
	while(timer->isRunning)
	{
		print.flush();	
		startFrame = std::chrono::system_clock::now();

		print.header();
		print.ren->renderTime(currentTimer);
		print.timer();
		print.characterStats(save->Char());
		
		std::cout << "animation: ";
		for(int i = 0; i < (int)animationTimer; ++i){
			std::cout << "=";
		}



		std::this_thread::sleep_for(std::chrono::milliseconds(1000/frames));
		if(*keyboardInput != "")
		{
			//now we have to change the setting of the statemachine. this will change wether the timer goes up or down 
			if(*keyboardInput == "up")
			{
				timer->SetState(TimerState::countUp);
				*keyboardInput = "";	
				std::thread thread_obj(&sleepfuntion, keyboardInput);
				thread_obj.detach();
			}else if(*keyboardInput == "down")
			{
				timer->SetState(TimerState::countDown);
				*keyboardInput = "";
				std::thread thread_obj(&sleepfuntion, keyboardInput);
				thread_obj.detach();
			}
		}

		endFrame = std::chrono::system_clock::now();
		deltaTime = std::chrono::duration<double, std::milli>(endFrame-startFrame).count();
		timer->Tick(currentTimer, deltaTime);

		//pseudo saving 
		exp += (deltaTime);
		if (exp >= 1000)
		{
			save->Char()->SetExp((int)(save->Char()->Exp()+exp/1000));
			save->Save(saveGame::SaveGameKeys::exp, save->GetKeyValue(saveGame::SaveGameKeys::exp));
			exp = 0;
			++animationTimer;
		}
		



		if(animationTimer >= 60){
			animationTimer=0;
		}
	};
}

int main (int argc, char *argv[]) {
	double worktimer = 10;
	double braketimer = 2;
	double countdown = 10000;
	double running = true;		
	Timer* timer = new Timer(TimerState::countDown);

	saveGame *mySave = new saveGame();
	mySave->Load();

	printer print;

	std::cout << "\033[2J \033[1H" <<"Starting PomoRPG... \n";

	std::cout << "welcome to your own liddle pomodoro timer \n \n";

	if(argc > 1)
	{	
		for (int i = 1; i < argc; ++i) {
			
			if(argc > 1 )
			{
				if(ARGV[0].compare(argv[i])==0)
				{
					//countUp
					timer->SetState(TimerState::countUp);
					timer->SetTime(0);
					worktimer = 0;
				}
				if(ARGV[1].compare(argv[i])==0)
				{
					//countdown
					timer->SetState(TimerState::countDown);
				}
			}

		}
	}else {
		timer->SetState(TimerState::countDown);
		std::cout << "how long do you want to work ? (minutes)";
		std::cin >> worktimer;

		std::cout << "how long do you want your brake to be? (minutes)";
		std::cin >> braketimer;

		std::cout << "\033[2J";
	}
	worktimer *= 60000;
	braketimer *= 60000;
	timer->isRunning = true;
	while(running)
	{
		countingTimer(worktimer, timer, mySave, print);
		//	notify
		//	breaketime
		std::cout << "\033[1J \033[1H" << "Press Enter key to continue...";
		std::cin.get();
		
		std::cout<<std::flush;
		/*
		countingTimer(braketimer);
		//	notify
		//	wait for user imput
		std::cout << "\033[1J \033[1H" << "Press Enter key to continue...";
		std::cin.get();
		*/
	}


	return 0;
}
