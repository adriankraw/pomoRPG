#include <cctype>
#include <csignal>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <memory>
#include <ostream>
#include <string>
#include <sys/resource.h>
#include <sys/signal.h>
#include <sys/ttycom.h>
#include <thread>

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

#include "Timer.cpp"
#include "printer.cpp"
#include "KeyCode.h"
#include "Commands.cpp"
#include "Game.cpp"
#include "logger.cpp"

int instanceID;

std::shared_ptr<std::string> keyboardInput = std::make_shared<std::string>();
std::thread inputReading;

logger keyboardLogger("keyboardlogger.log");
logger skillLogger("skilllogger.log");

struct termios tio_save;
struct winsize size;
void ttyreset()
{

	write (STDOUT_FILENO, "\e[?47l", 6);
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

void ttyinit(Game* game)
{
    tcgetattr(STDIN_FILENO,&tio_save);
    atexit(ttyreset);
    struct termios tio;
    tcgetattr(STDIN_FILENO,&tio);
    
    write (STDOUT_FILENO, "\e[?47h", 6);

    tio.c_lflag &= ~(IXON);
    tio.c_lflag &= ~(ECHO | ICANON | ISIG);

    tcsetattr(STDIN_FILENO,TCSANOW,&tio);
    winsize _size;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &_size)>= 0 )
    {
	game->SetWindowSize(_size.ws_col, _size.ws_row);
    }
    fflush(stdout);
    signal(SIGWINCH, signalHandler);

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
std::string TabComplete(std::string input)
{
	for(auto iter = Commands::commandsMap.begin(); iter != Commands::commandsMap.end(); iter++)
	{
		size_t pos = iter->second.find(input);
		if(pos!=iter->second.npos) return iter->second.substr(pos+input.size(), iter->second.npos);
	}
	return "";
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
			}else if(c == KeyCode::Btn::Tab)
			{
				*cinText += TabComplete(*cinText);
			}
			else {
				*cinText += c;
			}
		}
	}
	return;
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
	Game myGame = Game();
	
	myGame.Start();
	//websocketStart();

	ttyinit(&myGame);

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
					//countup
				}
				if(argument == Commands::commandsMap[Commands::Base::CountDown])
				{
					//countdown
				}
				if(argument == Commands::commandsMap[Commands::Base::timer])
				{
					myGame.globalTimer = &myGame.GetStopWatchByIndex(
						myGame.GetStopwatchIndex(argv[i+1])).GetcurrentTime();	
				}
				if(argument == Commands::commandsMap[Commands::Base::PrintAll])
				{
					myGame.gameManager.print.print_input = true;
					myGame.gameManager.print.print_stopwatches = true;
					myGame.gameManager.print.print_charsettings = true;
					myGame.gameManager.print.print_bigClock = true;
					myGame.gameManager.print.print_eventList = true;
					myGame.gameManager.print.print_fight = true;
				}
			}

		}
	}
	inputReading = std::thread(&sleepfuntion, keyboardInput);
	inputReading.detach();
	while(myGame.isRunning)
	{
		myGame.SetInput(keyboardInput);
		myGame.Update();
	}
	std::cout<<std::flush;
	std::cout << "\033[?25h";
	ttyreset();
	exit(1);
	return 0;
}

