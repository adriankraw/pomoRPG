#include <cctype>
#include <chrono>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <ostream>
#include <ratio>
#include <string>
#include <sys/resource.h>
#include <sys/signal.h>
#include <sys/ttycom.h>
#include <thread>
#include <vector>

#if defined(__APPLE__)
	#include <sys/termios.h>
	#include <termios.h>
	#include <sys/ioctl.h>
	#include <unistd.h>
#elif defined(_WIN32)
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

#define frames 30
#define actionTick 100
#define eventTick 5000

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

logger keyboardLogger("keyboardlogger.log");
logger skillLogger("skilllogger.log");

struct termios tio_save;
struct winsize size;
void ttyreset()
{
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &tio_save);
}

void signalHandler(int i)
{
    if(i == SIGWINCH)
    {
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
	fflush(stdout);
    }
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
    winsize _size;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &_size)>= 0 )
    {
	size = _size;
    }
    fflush(stdout);
    signal(SIGWINCH, signalHandler);

    write (STDOUT_FILENO, "\e[?47h", 6);
    //mouse Tracking
    //write (STDOUT_FILENO, "\e[?9h", 5);
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
void processInput(std::shared_ptr<std::string> keyboardInput, Time& globalTime, Timer* timer, saveGame* save, Printer& print)
{
	if(keyboardInput->back() == '\n')
	{
		std::string additionalInfo;
		keyboardInput->pop_back();
		if((*keyboardInput).find(KeyCode::Btn::Space) != std::string::npos)
		{
			additionalInfo = keyboardInput->substr(keyboardInput->find(KeyCode::Btn::Space)+1,keyboardInput->length());
			*keyboardInput = keyboardInput->substr(0,keyboardInput->find(KeyCode::Btn::Space));
		}if(*keyboardInput == Commands::commandsMap[Commands::help])
		{
			print.print_help = !print.print_help;
		}else if(*keyboardInput == Commands::commandsMap[Commands::up])
		{
			timer->SetState(TimerState::countUp);
		}else if(*keyboardInput == Commands::commandsMap[Commands::down])
		{
			timer->SetState(TimerState::countDown);
		}else if(*keyboardInput == Commands::commandsMap[Commands::save])
		{
			save->Save();
		}else if(*keyboardInput == Commands::commandsMap[Commands::timer])
		{
			keyboardLogger.log(logger::ErrorLevel::Warn,"changing timer to: "+ additionalInfo);
			//select a Timer
			if(!additionalInfo.empty())
			{
				globalTime.GetHour() = save->GetStopWatchByIndex(save->GetStopwatchIndex(additionalInfo))->GetcurrentTime()->GetHour();	
				globalTime.GetMinute() = save->GetStopWatchByIndex(save->GetStopwatchIndex(additionalInfo))->GetcurrentTime()->GetMinute();	
				globalTime.GetSeconds() = save->GetStopWatchByIndex(save->GetStopwatchIndex(additionalInfo))->GetcurrentTime()->GetSeconds();	
				globalTime.GetMili() = save->GetStopWatchByIndex(save->GetStopwatchIndex(additionalInfo))->GetcurrentTime()->GetMili();	
			}
		}else if(*keyboardInput == Commands::commandsMap[Commands::add])
		{
			if(!additionalInfo.empty())
			{
				save->AddStopwatch(additionalInfo);
			}
		}else if(*keyboardInput == Commands::commandsMap[Commands::pause] || *keyboardInput == Commands::commandsMap[Commands::stop])
		{
			if(!additionalInfo.empty())
			{
				keyboardLogger.log(logger::ErrorLevel::Warn,"stopped: "+ additionalInfo);
				if((size_t)save->GetStopwatchIndex(additionalInfo) != save->GetStopWatchList()->size())
				{
					keyboardLogger.log(logger::ErrorLevel::Info, "found watch!");
					save->GetStopWatchByIndex(save->GetStopwatchIndex(additionalInfo))->GetTimer()->Pause();
				}
			}else {
				timer->isPaused = true;
			}
		}else if(*keyboardInput == Commands::commandsMap[Commands::start] || *keyboardInput == Commands::commandsMap[Commands::resume])
		{
			if(!additionalInfo.empty())
			{
				if((size_t)save->GetStopwatchIndex(additionalInfo) != save->GetStopWatchList()->size())
				{
					save->GetStopWatchByIndex(save->GetStopwatchIndex(additionalInfo))->GetTimer()->UnPause();
				}
			}else {
				timer->isPaused = false;
			}
		}else if(*keyboardInput == Commands::commandsMap[Commands::bigclock])
		{
			print.print_bigClock = !print.print_bigClock;
		}else if(*keyboardInput == Commands::commandsMap[Commands::charsettings] || *keyboardInput == Commands::commandsMap[Commands::charstats])
		{
			print.print_charsettings = !print.print_charsettings;
		}else if(*keyboardInput == Commands::commandsMap[Commands::stopwatches])
		{
			print.print_stopwatches = !print.print_stopwatches;
		}else if(*keyboardInput == Commands::commandsMap[Commands::fight])
		{
			print.print_fight = !print.print_fight;
		}else if(*keyboardInput == Commands::commandsMap[Commands::eventlist])
		{
			print.print_eventList = !print.print_eventList;
		}else if(*keyboardInput == Commands::commandsMap[Commands::circle])
		{
			print.print_circle = !print.print_circle;
		}else if(*keyboardInput == Commands::commandsMap[Commands::exit])
		{
			running = false;
		}
		keyboardInput->clear();

	}
}
void ProcessFrame(Time &globalTimer, Timer *timer, saveGame *save, Printer &print)
{
	double frame = 0;
	double exp = 0;
	double eventTimer = 0;
	double actionTimer = 0;

	while(timer->isRunning && running)
	{
		processInput(keyboardInput, globalTimer, timer, save, print);

		print.Flush();	
		startFrame = std::chrono::system_clock::now();
		print.SetSize(size.ws_col, size.ws_row);
		print.Header();
		if(print.print_bigClock)
		{
			print.Timer(globalTimer);
		}
		if(print.print_charsettings)
		{
			print.CharacterStats(save->Char());
		}
		if(print.print_stopwatches)
		{
			size_t stopWatchListSize = save->GetStopWatchList()->size();
			for (size_t i = 0; i<stopWatchListSize; ++i) {
				if(save->GetStopWatchByIndex(i)->GetTimer()->isPaused)
				{
					//std::cout << "Stopped: "; // TODO: move to screenbuffer
				}else{
					//std::cout << "Going: ";
				}
				std::string t = save->GetStopWatchByIndex(i)->GetNameOfCorrespondingSkill();
				int maxCount = save->GetMaxFromStopwatchName(t);
				int seconds = save->GetStopWatchByIndex(i)->GetcurrentTime()->ConvertToSecondsForModulo(maxCount);			
				print.Bar(*save->GetStopWatchByIndex(i)->GetName(), seconds, maxCount);
			}
		}
		if(print.print_eventList)
		{
			print.EventsList(save->Char()->GetEvents());
		}
		if(print.print_circle)
		{
			print.Circle(frame);
		}
		if(print.print_fight)
		{

			std::vector<std::tuple<Character::CharEvent, void*>>* events = save->Char()->GetEvents();
			if(!events->empty())
			{
				auto* currentMonster = (Monster*)(std::get<1>(events->at(0)));
				print.OpenFightScreen(save->Char(),currentMonster);
			}
		}
		if(print.print_help)
		{
			print.Help();
		}
		print.PrintScreen();
		/* this has to be handled on a different Thread */
		if(print.print_input)
		{
			std::cout << "> " << keyboardInput->c_str() <<"\033[48;5;255m \033[0m \033[0K";
		}
		std::cout << "\033[?25l"; //Make Cursor invisible

		//logic
		if(!timer->isPaused) {

			std::vector<stopwatch>* stopwatchList = save->GetStopWatchList();
			size_t stopwatchListSize = stopwatchList->size();
			for (size_t i = 0; i<stopwatchListSize; ++i) {
				if(save->GetStopWatchByIndex(i)->GetTimer()->isPaused == false)
				{
					save->GetStopWatchByIndex(i)->GetTimer()->Tick(TimerState::countUp, *save->GetStopWatchByIndex(i)->GetcurrentTime(), deltaTime);
				}

				if(eventTimer >= eventTick) // every 5 seconds 
				{
					keyboardLogger.log(logger::ErrorLevel::Dbg, "EventTriggered");
					Character::CharEvent charEvent = save->Char()->GetRandomEvent();
					switch(charEvent) {
						case Character::CharEvent::Fight:
						{
							Area* area = save->Char()->CurrentArea();
							Monster* monster = area->Getmonster();
							//Adding all the monsters into a queue
							//keyboardLogger.log(logger::ErrorLevel::Dbg, "found: Fight Event"+*monster->GetName());

							save->Char()->AddMonsterToEventMap(Character::CharEvent::Fight, monster);
							//print.OpenFightScreen(save->Char(), area, monster); This doesnt make sense right now
						}
						break;
						case Character::CharEvent::Chest:
						{
							Area* area = save->Char()->CurrentArea();
							Rarity::Level rarity = area->GetRandomRarety();
							int itemCode{0}, itemAmount{0};
							area->RollItem(&rarity, itemCode, itemAmount);
							save->Char()->AddUserItemToEventMap(Character::CharEvent::Chest, new ItemDrop(itemCode, itemAmount));
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

			}
			if(actionTimer >= save->Char()->skillCooldown * actionTick)
			{
				/* Handle Events */
				std::vector<std::tuple<Character::CharEvent, void*>>* events = save->Char()->GetEvents();
				if(print.print_fight && !events->empty())
				{
					switch(std::get<0>(events->at(0)))
					{
						case Character::CharEvent::Fight:
						{
							auto* currentMonster = (Monster*)(std::get<1>(events->at(0)));
							auto* skill = save->Char()->GetSkill();
							//skillLogger.log(logger::ErrorLevel::Warn, skill->name + " activated");
							save->Char()->skillCooldown = skill->cost;
							skill->Activate(save->Char(), currentMonster);
							save->Char()->GetAttacked(*currentMonster->GetLevel());

							if(*currentMonster->GetLife() <= 0)
							{
								delete currentMonster;
								events->erase(events->begin());
								//The player should get something for slaying an enemy
								save->Char()->SetExp(save->Char()->Exp() + 100);
							}
						}
						break;
						case Character::CharEvent::Chest:
						{
							auto* itemDrop = (ItemDrop*)(std::get<1>(events->at(0)));
							save->Char()->AddToInventory(itemDrop->itemCode, itemDrop->itemAmount);
							save->Char()->skillCooldown = 1;
							delete itemDrop;
							events->erase(events->begin());
						}
						break;
						case Character::CharEvent::Encounter:
						{
						}
						break;
						case Character::CharEvent::Nothing:
						{
						}
						break;
					}

				}
				actionTimer = 0;
			}

			std::this_thread::sleep_for(std::chrono::milliseconds(1000/frames));
			endFrame = std::chrono::system_clock::now();
			deltaTime = std::chrono::duration<double, std::milli>(endFrame-startFrame).count();
			timer->Tick(globalTimer, deltaTime); //this is to tick the BIG clock. ITS NOT SAVED AS A WATCH
			frame += 0.5f;
			exp += (deltaTime);
			eventTimer += deltaTime;
			actionTimer += deltaTime;
			if(exp >= 1000)
			{
				save->Char()->SetExp((int)(save->Char()->Exp()+exp/1000)*save->Char()->Expmultiplier());
				exp = 0;
			}
		}else{
			std::this_thread::sleep_for(std::chrono::milliseconds(1000/frames));
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

	Time worktimer{0,0,0,0};
	running = true;

	Timer* timer = new Timer(TimerState::countDown);
	timer->SetState(TimerState::paused);
	saveGame *mySave = new saveGame();
	mySave->Load();

	Game myGame = Game();
	//websocketStart();

	Printer print;

	ttyinit();

	std::cout << "\033[2J \033[1H" <<"Starting PomoRPG... \n";
	std::cout << "welcome to your own liddle pomodoro timer \n \n";
	if(argc > 1)
	{	
		for (int i = 1; i < argc; ++i) {
			
			if(argc > 1 )
			{
				std::string argument = argv[i];
				if( argument.front() == '-')
				{
					argument = argument.substr(1, argument.length()-1);
				}
				if(argument == Commands::commandsMap[Commands::Base::CountUp])
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
						worktimer.resetTime();
					}
				}
				if(argument == Commands::commandsMap[Commands::Base::CountDown])
				{
					//countdown
					timer->SetState(TimerState::countDown);
				}
				if(argument == Commands::commandsMap[Commands::Base::CountDown])
				{
					//countdown
					timer->SetState(TimerState::countDown);
				}
				if(argument == Commands::commandsMap[Commands::Base::PrintAll])
				{
					print.print_input = true;
					print.print_stopwatches = true;
					print.print_charsettings = true;
					print.print_bigClock = true;
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
	write (STDOUT_FILENO, "\e[?47l", 6);
	std::cout << "\033[?25h";
	exit(1);
	return 0;
}

