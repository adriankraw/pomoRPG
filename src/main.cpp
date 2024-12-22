#include <cctype>
#include <chrono>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <ostream>
#include <ratio>
#include <string>
#include <thread>
#include <stdio.h>
#include <vector>

#if defined(Darwin) || true
	#include <sys/termios.h>
	#include <termios.h>
	#include <unistd.h>
#elif defined(Windows)
	#include <windows.h>
#endif

#if defined(NETWORK)
	#include <boost/beast/core.hpp>
	#include <boost/beast/websocket.hpp>
	#include <boost/asio/connect.hpp>
	#include <boost/asio/ip/tcp.hpp>
	#include "boost/beast/websocket/rfc6455.hpp"
	#include "boost/beast/websocket/stream.hpp"
#endif

#define frames 60

#include "Character.cpp"
#include "saveGame.cpp"
#include "Timer.cpp"
#include "printer.cpp"
#include "KeyCode.h"
#include "Commands.cpp"
#include "Game.cpp"
#include "logger.cpp"

bool running = true;

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
void ttyreset()
{
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &tio_save);
}

void ttyinit()
{
    tcgetattr(STDIN_FILENO,&tio_save);
    atexit(ttyreset);
    struct termios tio;
    tcgetattr(STDIN_FILENO,&tio);

    tio.c_lflag &= ~(IXON);
    tio.c_lflag &= ~(ECHO | ICANON | ISIG);

    tcsetattr(STDIN_FILENO,TCSANOW,&tio);
}

int getkey()
{
    unsigned char buf[1];
    int len;

    len = read(STDIN_FILENO,buf,1);

    if (len > 0)
        len = buf[0];

    return len;
}
void sleepfuntion(std::shared_ptr<std::string> cinText)
{
	while(true)
	{
		if(char c = getkey())
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
			}else if(c == KeyCode::Btn::CTRL_C)
			{
				*cinText += "exit\n";
			}
			else {
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
		if((*keyboardInput).find(KeyCode::Btn::Space) != std::string::npos)
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
				currentTime.GetHour() = save->GetStopWatchByIndex(save->GetStopwatchIndex(additionalInfo))->GetcurrentTime()->GetHour();	
				currentTime.GetMinute() = save->GetStopWatchByIndex(save->GetStopwatchIndex(additionalInfo))->GetcurrentTime()->GetMinute();	
				currentTime.GetSeconds() = save->GetStopWatchByIndex(save->GetStopwatchIndex(additionalInfo))->GetcurrentTime()->GetSeconds();	
				currentTime.GetMili() = save->GetStopWatchByIndex(save->GetStopwatchIndex(additionalInfo))->GetcurrentTime()->GetMili();	
			}
		}else if(*keyboardInput == "add")
		{
			if(additionalInfo != "")
			{
				save->AddStopwatch(additionalInfo);
			}
		}else if(*keyboardInput == "pause" || *keyboardInput == "stop")
		{
			if(additionalInfo != "")
			{
				keyboardLogger.log(logger::ErrorLevel::Warn,"stopped: "+ additionalInfo);
				if(save->GetStopwatchIndex(additionalInfo) != save->GetStopWatchList()->size())
				{
					keyboardLogger.log(logger::ErrorLevel::Info, "found watch!");
					save->GetStopWatchByIndex(save->GetStopwatchIndex(additionalInfo))->GetTimer()->Pause();
				}
			}else {
				timer->isPaused = true;
			}
		}else if(*keyboardInput == "start" || *keyboardInput == "resume")
		{
			if(additionalInfo != "")
			{
				if(save->GetStopwatchIndex(additionalInfo) != save->GetStopWatchList()->size())
					save->GetStopWatchByIndex(save->GetStopwatchIndex(additionalInfo))->GetTimer()->UnPause();
			}else {
				timer->isPaused = false;
			}
		}else if(*keyboardInput == "bigclock")
		{
			print_bigClock = !print_bigClock;
		}else if(*keyboardInput == "charsettings")
		{
			print_charsettings = !print_charsettings;
		}else if(*keyboardInput == "stopwatches")
		{
			print_stopwatches = !print_stopwatches;
		}else if(*keyboardInput == "exit")
		{
			running = false;
		}
		keyboardInput->clear();

	}
}
void ProcessFrame(Time &currentTime, Timer *timer, saveGame *save, printer &print)
{
	double exp = 0;
	double eventTimer = 0;

	while(timer->isRunning && running)
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

		std::vector<stopwatch>* stopwatchList = save->GetStopWatchList();
		for (int i = 0; i<stopwatchList->size(); ++i) {
			if(save->GetStopWatchByIndex(i)->GetTimer()->isPaused == false)
			{
				save->GetStopWatchByIndex(i)->GetTimer()->Tick(TimerState::countUp, *save->GetStopWatchByIndex(i)->GetcurrentTime(), deltaTime);
			}

			if(eventTimer >= 5000) // every 5 seconds 
			{
				keyboardLogger.log(logger::ErrorLevel::Dbg, "EventTriggered");
				Character::CharEvent charEvent = save->Char()->GetRandomEvent();
				switch(charEvent) {
					case Character::CharEvent::Fight:
					{
						Area* area = save->Char()->CurrentArea();
						Monster* monster = area->Getmonster();
						//Adding all the monsters into a queue
						keyboardLogger.log(logger::ErrorLevel::Dbg, "found: Fight Event"+*monster->GetName());

						save->Char()->AddMonsterToEventMap(Character::CharEvent::Fight, monster);
						//print.OpenFightScreen(save->Char(), area, monster); This doesnt make sense right now
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
						//coud be: you encounter some new place;
						//This is how you are supposed to find the RaidBoss of this Area;
					break;
					case Character::CharEvent::Nothing:
						//PlaceHolder for Nothing was found, Good luck next Time;
						;
					break;
					default:
					break;
				}
				eventTimer = 0;
			}
			if(print_stopwatches)
			{
				if(save->GetStopWatchByIndex(i)->GetTimer()->isPaused)
				{
					std::cout << "Stopped: ";
				}else{
					std::cout << "Going: ";
				}
				print.Bar(*save->GetStopWatchByIndex(i)->GetName(), save->GetStopWatchByIndex(i)->GetcurrentTime()->GetSeconds());
			}
		}

		std::vector<std::tuple<Character::CharEvent, void*>>* events = save->Char()->GetEvents();
		print.EventsList(events);

		/* Handle Events */
		if(events->size() > 0)
		{
			Monster* currentMonster = (Monster*)(std::get<1>(events->at(0)));
			currentMonster->GetAttacked(save->Char()->Atk());
			if(*currentMonster->GetLife() <= 0)
			{
				delete currentMonster;
				events->erase(events->begin());
				//The player should get something for slaying an enemy
				save->Char()->SetExp(save->Char()->Exp() + 100);
			}

			std::cout << "--------------------------Player---------------------------" << std::endl;
			std::cout << "Name: " << save->Char()->Name() << std::endl;
			std::cout << "LVL: " << save->Char()->Lvl() << std::endl;
			std::cout << "Life: " << save->Char()->Life()<< std::endl;
			std::cout << "Atk: " << save->Char()->Atk() << std::endl;

			std::cout << "--------------------------Monster--------------------------" << std::endl;
			std::cout << "Name: " << *currentMonster->GetName() << std::endl;
			std::cout << "LVL: " << *currentMonster->GetLevel() << std::endl;
			std::cout << "Life: " << *currentMonster->GetLife() << std::endl;
			std::cout << "-----------------------------------------------------------" << std::endl;
		}
		/* this has to be handled on a different Thread */
		if(print_input)
		{
			std::cout << "> " << keyboardInput->c_str() <<"\033[48;5;255m \033[0m";
		}
		std::cout << "\033[?25l" << std::endl; //Make Cursor invisible


		std::this_thread::sleep_for(std::chrono::milliseconds(1000/frames));

		endFrame = std::chrono::system_clock::now();
		deltaTime = std::chrono::duration<double, std::milli>(endFrame-startFrame).count();
		timer->Tick(currentTime, deltaTime); //this is to tick the BIG clock. ITS NOT SAVED AS A WATCH
		exp += (deltaTime);
		eventTimer += deltaTime;
		if(exp >= 1000)
		{
			save->Char()->SetExp((int)(save->Char()->Exp()+exp/1000)*save->Char()->Expmultiplier());
			exp = 0;
		}
	};
}

#if defined(NETWORK)
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
#endif


int main (int argc, char *argv[]) {
	
	instanceID = std::rand();

	Time worktimer;
	double braketimer = 2;
	double countdown = 10000;
	running = true;

	Timer* timer = new Timer(TimerState::countDown);

	saveGame *mySave = new saveGame();
	mySave->Load();

	Game myGame = Game();
	//websocketStart();

	printer print;

	ttyinit();

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
						stopwatch* foundwatch = mySave->GetStopWatchByIndex(mySave->GetStopwatchIndex(argv[i+1]));
						
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
		std::cout<<std::flush;
	}
	std::cout << "\033[?25h";
	exit(1);
	return 0;
}

