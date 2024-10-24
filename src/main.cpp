#include <cctype>
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
#include "Character.cpp"
#include "boost/beast/websocket/rfc6455.hpp"
#include "boost/beast/websocket/stream.hpp"

#define frames 60

#include "saveGame.cpp"
#include "Timer.cpp"
#include "printer.cpp"
#include "KeyCode.h"
#include "Commands.cpp"
#include "Game.cpp"
#include "logger.cpp"

std::chrono::time_point<std::chrono::system_clock> startFrame;
std::chrono::time_point<std::chrono::system_clock> endFrame;
double deltaTime(0);
int instanceID;

std::shared_ptr<std::string> keyboardInput = std::make_shared<std::string>();
std::thread inputReading;

//printer settings
bool print_bigClock = true;
bool print_charsettings = false;
bool print_input = true;
bool print_stopwatches = false;

logger keyboardLogger("keyboardlogger.log");

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
void processInput(std::shared_ptr<std::string> keyboardInput, Time& currentTime, Timer* timer, saveGame* save)
{
	if(keyboardInput->back() == '\n')
	{
		std::string additionalInfo = "";
		keyboardInput->pop_back();
		keyboardLogger.log(logger::ErrorLevel::Info, *keyboardInput);
		if((*keyboardInput).find(KeyCode::Space))
		{
			additionalInfo = keyboardInput->substr(keyboardInput->find(KeyCode::Btn::Space)+1,keyboardInput->length());
			*keyboardInput = keyboardInput->substr(0,keyboardInput->find(KeyCode::Btn::Space));
		}
		if(*keyboardInput == "up")
		{
			timer->SetState(TimerState::countUp);
		}else if(*keyboardInput == "down")
		{
			timer->SetState(TimerState::countDown);
		}else if(*keyboardInput == "save")
		{
			save->Save();
		}else if(*keyboardInput == "timer")
		{
			keyboardLogger.log(logger::ErrorLevel::Warn,"changing timer to: "+ additionalInfo);
			//select a Timer
			if(additionalInfo != "")
			{
				currentTime.GetHour() = save->GetStopwatchIndex(additionalInfo)->GetcurrentTime()->GetHour();	
				currentTime.GetMinute() = save->GetStopwatchIndex(additionalInfo)->GetcurrentTime()->GetMinute();	
				currentTime.GetSeconds() = save->GetStopwatchIndex(additionalInfo)->GetcurrentTime()->GetSeconds();	
				currentTime.GetMili() = save->GetStopwatchIndex(additionalInfo)->GetcurrentTime()->GetMili();	
			}
		}else if(*keyboardInput == "add")
		{
			if(additionalInfo != "")
			{
				save->AddStopwatch(additionalInfo);
			}
		}else if(*keyboardInput == "pause" || *keyboardInput == "stop")
		{
			timer->isPaused = true;
		}else if(*keyboardInput == "start" || *keyboardInput == "resume")
		{
			timer->isPaused = false;
		}else if(*keyboardInput == "bigclock")
		{
			print_bigClock = !print_bigClock;
		}else if(*keyboardInput == "charsettings")
		{
			print_charsettings = !print_charsettings;
		}else if(*keyboardInput == "stopwatches")
		{
			print_stopwatches = !print_stopwatches;
		}
		keyboardInput->clear();

	}
}
void ProcessFrame(Time &currentTime, Timer *timer, saveGame *save, printer &print)
{
	ttyinit(STDIN_FILENO);

	double exp = 0;
	double animationTimer = 0;
	//ask for input

	while(timer->isRunning)
	{
		processInput(keyboardInput, currentTime, timer, save);

		if(timer->isPaused) {
			continue;
		}
		print.flush();	
		startFrame = std::chrono::system_clock::now();
		print.header();
		print.ren->renderTime(currentTime);
		if(print_bigClock)
		{
			print.timer();
		}
		if(print_charsettings)
		{
			print.characterStats(save->Char());
		}

		std::vector<stopwatch>& stopwatchList = save->GetStopWatchList();
		for (int i = 0; i<stopwatchList.size(); ++i) {
			stopwatchList[i].GetTimer().Tick(TimerState::countUp, *stopwatchList[i].GetcurrentTime(), deltaTime);

			if(stopwatchList[i].GetcurrentTime()->GetSeconds()%60 == 0)
			{
				Character::CharEvent charEvent = save->Char()->GetRandomEvent();
				switch(charEvent) {
					case Character::CharEvent::Fight :
					{
						Area* area = save->Char()->CurrentArea();
						Monster* monster = area->Getmonster();
						print.OpenFightScreen(save->Char(), area, monster);
					}
					break;
					case Character::CharEvent::Chest:
					{
						Area* area = save->Char()->CurrentArea();
						Rarity::Level rarity = area->GetRandomRarety();
						int itemCode(0), itemAmount(0);
						area->RollItem(&rarity, itemCode, itemAmount);
						save->Char()->AddUserItem(itemCode, itemAmount);
					}
					break;
					case Character::CharEvent::Encounter:
					break;
					case Character::CharEvent::Nothing:
						//PlaceHolder for Nothing was found, Good luck next Time;
						;
					break;
					default:
					break;
				}
			}
			if(print_stopwatches)
			{
				print.Bar(stopwatchList[i].GetName(), stopwatchList[i].GetcurrentTime()->GetSeconds());
			}
		}

		if(print_input)
		{
			std::cout << "> " << keyboardInput->c_str();
		}
		std::cout << "" << std::endl;


		std::this_thread::sleep_for(std::chrono::milliseconds(1000/frames));

		endFrame = std::chrono::system_clock::now();
		deltaTime = std::chrono::duration<double, std::milli>(endFrame-startFrame).count();
		timer->Tick(currentTime, deltaTime); //this is to tick the BIG clock. ITS NOT SAVED AS A WATCH
		exp += (deltaTime);
		if(exp >= 1000)
		{
			save->Char()->SetExp((int)(save->Char()->Exp()+exp/1000)*save->Char()->Expmultiplier());
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

	Time worktimer;
	double braketimer = 2;
	double countdown = 10000;
	double running = true;

	Timer* timer = new Timer(TimerState::countDown);

	saveGame *mySave = new saveGame();
	mySave->Load();

	Game myGame = Game();
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
						
						Time *found;
						found = foundwatch->GetcurrentTime();
						timer->SetTime(found->GetHour(),found->GetMinute(),found->GetSeconds(),found->GetMili());
						worktimer = *found;
			
					}else {
						timer->SetTime(0, 0, 0, 0);
						worktimer = *new Time(0,0,0,0);
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
				if(Commands::CommandsMap[Commands::Base::PrintAll].compare(argv[i])==0)
				{
					print_input = true;
					print_stopwatches = true;
					print_charsettings = true;
					print_bigClock = true;
				}
			}

		}
	}
	timer->isRunning = true;
	inputReading = std::thread(&sleepfuntion, keyboardInput);
	inputReading.detach();
	while(running)
	{
		ProcessFrame(worktimer, timer, mySave, print);
		//	notify
		//	breaketime
		std::cout << "\033[1J \033[1H" << "Press Enter key to continue...";
		std::cin.get();
		
		std::cout<<std::flush;
		/*
		ProcessFrame(braketimer);
		//	notify
		//	wait for user imput
		std::cout << "\033[1J \033[1H" << "Press Enter key to continue...";
		std::cin.get();
		*/
	}


	return 0;
}

