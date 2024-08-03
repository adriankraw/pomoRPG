#include <algorithm>
#include <array>
#include <charconv>
#include <chrono>
#include <complex>
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
#include <sstream>
#include <string>
#include <sys/_types/_uintptr_t.h>
#include <thread>
#include <stdio.h>
#include <vector>

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include "boost/beast/http/message.hpp"
#include "boost/beast/http/string_body.hpp"
#include "boost/beast/websocket/rfc6455.hpp"
#include "boost/beast/websocket/stream.hpp"

#define frames 60

#include "saveGame.cpp"
#include "Timer.cpp"
#include "printer.cpp"

std::chrono::time_point<std::chrono::system_clock> startFrame;
std::chrono::time_point<std::chrono::system_clock> endFrame;
double deltaTime(0);
int instanceID;

std::vector<std::string> ARGV = {"-countUp","-countDown","-time","-coreAdress"};

std::shared_ptr<std::string> keyboardInput = std::make_shared<std::string>();

std::vector<double> idleTimerVector;

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
		
		for (int i = 0; i<idleTimerVector.size(); ++i) {
			timer->Tick(TimerState::countUp, idleTimerVector[i], deltaTime);
			double testdouble = idleTimerVector[i]/1000;
			print.Bar("timer: ", testdouble);
		}

		std::cout << ""<< std::endl;


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
			}else if(*keyboardInput == "save")
			{
				save->Save(saveGame::SaveGameKeys::exp, save->GetKeyValue(saveGame::SaveGameKeys::exp));
			}
		}

		endFrame = std::chrono::system_clock::now();
		deltaTime = std::chrono::duration<double, std::milli>(endFrame-startFrame).count();
		timer->Tick(currentTimer, deltaTime);

		exp += (deltaTime);
		if(exp >= 1000)
		{
			save->Char()->SetExp((int)(save->Char()->Exp()+exp/1000));
			exp = 0;
		}
		
		if(idleTimerVector[0] >= 10000)
		{
			idleTimerVector[0] = 0;
		}
		if(idleTimerVector[1] >= 5000)
		{
			idleTimerVector[1] = 0;
		}
	};
}

void websocketStart()
{
	std::string host("127.0.0.1");
	auto const port("8080");
	boost::asio::io_context ioc;
	boost::asio::ip::tcp::resolver ioc_resolver{ioc};
	boost::beast::websocket::stream<boost::asio::ip::tcp::socket> ws{ioc};
	auto const results = ioc_resolver.resolve(host, port);        
        auto ep = boost::asio::connect(ws.next_layer(), results);

        host += ':' + std::to_string(ep.port());
	std::string ip(results->endpoint().address().to_string());

        ws.set_option(boost::beast::websocket::stream_base::decorator(
            [&ep, &ip](boost::beast::websocket::request_type& req)
            {
                req.set(boost::beast::http::field::user_agent,
                    std::string(BOOST_BEAST_VERSION_STRING) +
                        " websocket-client-coro");
		req.set("instanceID", std::to_string(instanceID));
		req.set("ip",ip);
            })
	);

        ws.handshake(host, "/");

        // Send the message
        ws.write(boost::asio::buffer(std::string("text")) );

        // This buffer will hold the incoming message
	boost::beast::flat_buffer buffer;

        // Read a message into our buffer
        ws.read(buffer);

        // Close the WebSocket connection
        ws.close(boost::beast::websocket::close_code::normal);

        // If we get here then the connection is closed gracefully

        // The make_printable() function helps print a ConstBufferSequence
        std::cout << boost::beast::make_printable(buffer.data()) << std::endl;
}

int main (int argc, char *argv[]) {
	std::srand(std::time(nullptr));
	instanceID = std::rand();

	double worktimer = 10;
	double braketimer = 2;
	double countdown = 10000;
	double running = true;

	Timer* timer = new Timer(TimerState::countDown);

	saveGame *mySave = new saveGame();
	mySave->Load();

	//websocketStart();

	float searchingAreaTimer(0);
	float dungeonAreaTimer(0);

	idleTimerVector.push_back(searchingAreaTimer);
	idleTimerVector.push_back(dungeonAreaTimer);

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
