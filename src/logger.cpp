#pragma once

#include <ctime>
#include <fstream>
#include <iomanip>
#include <ios>
#include <iostream>
#include <string>
#include <map>

#if defined(NETWORK)
	#include <sys/wait.h>
#endif

class logger {
public:
	enum ErrorLevel{
		Info,
		Dbg,
		Warn,
		Err
	};
	std::map<logger::ErrorLevel, std::string> ErrorLebelString{
		{logger::ErrorLevel::Info, "[INFO]"},
		{logger::ErrorLevel::Dbg, "[DEBUG]"},
		{logger::ErrorLevel::Warn, "[WARN]"},
		{logger::ErrorLevel::Err, "[ERR]"},
	};
public:
	logger(std::string);
	logger(logger &&) = default;
	logger &operator=(logger &&) = default;
	~logger();

	void log(ErrorLevel, std::string);

private:
	std::string path;
	std::ofstream loggerFile;
};

logger::logger(std::string path) {
	logger::path = path;

	loggerFile.open(path, std::ios::out | std::ios::trunc);

	if(loggerFile.is_open())
	{
		std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		loggerFile << "logger created:" << std::put_time(std::localtime(&now), "%F %T.\n");	
		loggerFile.flush();
	}else if(loggerFile.fail()){
		std::cerr << loggerFile.fail() << std::endl;
	}
	loggerFile.close();
}

void logger::log(logger::ErrorLevel errLevel, std::string text)
{
	if(!loggerFile.is_open())
	{
		loggerFile.open(path, std::ios::out | std::ios_base::app);
	}
	if(loggerFile.is_open())
	{
		std::string line = "";
		std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		std::string timestamp = std::ctime(&now);
		timestamp.pop_back(); // removing \n from timestamp

		line.append(timestamp);
		line.append(": ");
		line.append(ErrorLebelString[errLevel]);
		line.append(" -");
		line.append(text);
		line.append("\n");
		loggerFile << line;
		loggerFile.flush();
	}
	loggerFile.close();
}

logger::~logger() {
	if(loggerFile.is_open())
	{
		loggerFile.close();
	}
}
