#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <ostream>
#include <ratio>
#include <string>
#include <sys/termios.h>
#include <thread>
#include <stdio.h>
#include <unistd.h>
#include <vector>
#include <termios.h>

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include "boost/beast/websocket/rfc6455.hpp"
#include "boost/beast/websocket/stream.hpp"

#define frames 60

#include "saveGame.cpp"
#include "Timer.cpp"
#include "printer.cpp"
#include "KeyCode.h"
#include "Commands.cpp"

std::chrono::time_point<std::chrono::system_clock> startFrame;
std::chrono::time_point<std::chrono::system_clock> endFrame;
double deltaTime(0);
int instanceID;

std::shared_ptr<std::string> keyboardInput = std::make_shared<std::string>();
std::thread inputReading;

struct termios tio_save;
void ttyinit(int fd)
{
    struct termios tio;
    tcgetattr(fd,&tio);
    tio_save = tio;

    tio.c_lflag &= ~(ECHO | ICANON);

    tcsetattr(fd,TCSANOW,&tio);
}

int getkey(int fd)
{
    unsigned char buf[1];
    int len;

    len = read(fd,buf,1);

    if (len > 0)
        len = buf[0];

    return len;
}
void sleepfuntion(std::shared_ptr<std::string> cinText)
{
	while(true)
	{
		if(char c = getkey(STDIN_FILENO))
			{
			if(c == KeyCode::Btn::Enter)
			{
				*cinText += c;
			}else if(c == KeyCode::Btn::Backspace)
			{
				if(!cinText->empty())
				{
					cinText->pop_back();
				}
			}else {
				*cinText += c;
			}
		}
	}
	return;
}
void countingTimer(double &currentTimer, Timer *timer, saveGame *save, printer &print)
{
	ttyinit(STDIN_FILENO);

	double exp = 0;
	double animationTimer = 0;
	//ask for input

	while(timer->isRunning)
	{
		print.flush();	
		startFrame = std::chrono::system_clock::now();
		print.header();
		print.ren->renderTime(currentTimer);
		print.timer();
		print.characterStats(save->Char());
		
		std::vector<stopwatch>& stopwatchList =  save->GetStopWatchList();
		for (int i = 0; i<stopwatchList.size(); ++i) {
			stopwatchList[i].GetTimer().Tick(TimerState::countUp, stopwatchList[i].GetcurrentTime(), deltaTime);
			print.Bar(stopwatchList[i].GetName(), stopwatchList[i].GetcurrentTime());
		}
		
		std::cout << "> " << keyboardInput->c_str();
		std::cout << "" << std::endl;


		std::this_thread::sleep_for(std::chrono::milliseconds(1000/frames));
		if(keyboardInput->back() == '\n')
		{
			std::string additionalInfo = "";
			if(keyboardInput->find(KeyCode::Space))
			{
				additionalInfo = keyboardInput->substr(keyboardInput->find(KeyCode::Btn::Space)+1,keyboardInput->length());
			}
			//keyboardInput has to checked for spaces to
			keyboardInput->pop_back();
			//now we have to change the setting of the statemachine. this will change wether the timer goes up or down 
			if(*keyboardInput == "up")
			{
				timer->SetState(TimerState::countUp);
				keyboardInput->clear();	
			}else if(*keyboardInput == "down")
			{
				timer->SetState(TimerState::countDown);
				keyboardInput->clear();	
			}else if(*keyboardInput == "save")
			{
				save->Save();
				keyboardInput->clear();	
			}else if(*keyboardInput == "timer")
			{
				//select a Timer
				if(additionalInfo != "")
				{
					
				}
			}else if(*keyboardInput == "add")
			{
				if(additionalInfo != "")
				{
					save->AddStopwatch(additionalInfo);
				}
				keyboardInput->clear();
			}else if(*keyboardInput == "pause" || *keyboardInput == "stop")
			{
				timer->isRunning = false;
				keyboardInput->clear();
			}else if(*keyboardInput == "start" || *keyboardInput == "resume")
			{
				timer->isRunning = true;
				keyboardInput->clear();
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
	
	instanceID = std::rand();

	double worktimer = 10;
	double braketimer = 2;
	double countdown = 10000;
	double running = true;

	Timer* timer = new Timer(TimerState::countDown);

	saveGame *mySave = new saveGame();
	mySave->Load();

	//websocketStart();

	printer print;

	std::cout << "\033[2J \033[1H" <<"Starting PomoRPG... \n";
	std::cout << "welcome to your own liddle pomodoro timer \n \n";
	if(argc > 1)
	{	
		for (int i = 1; i < argc; ++i) {
			
			if(argc > 1 )
			{
				if(Commands::CommandsMap[Commands::Base::CountUp].compare(argv[i])==0)
				{
					//countUp
					timer->SetState(TimerState::countUp);
					if(argc>2)
					{
						auto foundwatch = mySave->GetStopwatchIndex(argv[i+1]);
						
						if(foundwatch == NULL)
						{
							foundwatch = mySave->AddStopwatch(argv[i+1]);
						}
						
						std::cout << foundwatch->GetName() << std::endl;
						timer->SetTime(foundwatch->GetcurrentTime());
						worktimer = foundwatch->GetcurrentTime();
					}else {
						timer->SetTime(0);
						worktimer = 0;
					}
				}
				if(Commands::CommandsMap[Commands::Base::CountDown].compare(argv[i])==0)
				{
					//countdown
					timer->SetState(TimerState::countDown);
				}
				if(Commands::CommandsMap[Commands::Base::CountDown].compare(argv[i])==0)
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

	timer->isRunning = true;

	inputReading = std::thread(&sleepfuntion, keyboardInput);
	inputReading.detach();

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
