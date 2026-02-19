#pragma once
#include <array>
#include <string>
#include <list>
#include "Time.cpp"

#define stringarrayResolution 5

class render {
public:
	render();
	render(render &&) = default;
	render(const render &) = default;
	render &operator=(render &&) = default;
	render &operator=(const render &) = default;
	~render();

	std::array<std::string, stringarrayResolution> result{};

	std::array<std::string, stringarrayResolution> renderTime(Time &currentTime)
	{
		//return a string that got modified into a time 
		std::list<int> splitTime;
		int time = currentTime.GetHour();
		int mili = currentTime.GetMili();
		int minutes = currentTime.GetMinute();
		int seconds = currentTime.GetSeconds();
		time = mili + (seconds*1000) + (minutes*100000) + (time* 10000000);
		while (time >0)
		{
			splitTime.push_front(time % 10);
			time = (time /10);
		}
		size_t resultSize = result.size();
		for(size_t i = 0; i < resultSize; ++i)
		{
			result[i] = "";
		}
		while(splitTime.size()>0) {
			for(size_t r = 0; r < result.size(); ++r)
			{
				(result[r]).append(" ");
			}
			for(size_t s = 0; s < result.size(); ++s)
			{
				auto front = splitTime.front();
				auto number = numbers(front);
				(result[s]).append(number[s]);
			}
			if(splitTime.size() == 8 || splitTime.size() == 6 || splitTime.size() == 4) 
			{
				for(size_t r = 0; r < result.size(); ++r)
				{
					(result[r]).append(number_space[r]);
				}
			}
			splitTime.pop_front();
		}
		return result;
	}

private:
	std::array<std::string, stringarrayResolution> number_zero = {
		"████",
		"█  █",
		"█  █",
		"█  █",
		"████"
	};
	std::array<std::string, stringarrayResolution> number_one = {
		"   █",
		"   █",
		"   █",
		"   █",
		"   █"
	};
	std::array<std::string, stringarrayResolution> number_two = {
		"████",
		"█  █",
		"  █ ",
		" █  ",
		"████"
	};
	std::array<std::string, stringarrayResolution> number_three = {
		"████",
		"   █",
		" ███",
		"   █",
		"████"
	};
	std::array<std::string, stringarrayResolution> number_four = {
		"█  █",
		"█  █",
		"████",
		"   █",
		"   █"
	};
	std::array<std::string, stringarrayResolution> number_five = {
		"████",
		"█   ",
		"████",
		"   █",
		"████"
	};
	std::array<std::string, stringarrayResolution> number_six = {
		"████",
		"█   ",
		"████",
		"█  █",
		"████"
	};
	std::array<std::string, stringarrayResolution> number_seven = {
		"████",
		"█  █",
		"   █",
		"   █",
		"   █",
	};
	std::array<std::string, stringarrayResolution> number_eight = {
		"████",
		"█  █",
		"████",
		"█  █",
		"████"
	};
	std::array<std::string, stringarrayResolution> number_nine = {
		"████",
		"█  █",
		"████",
		"   █",
		"████"
	};
	std::array<std::string, stringarrayResolution> number_space = {
		"   ",
		" █ ",
		"   ",
		" █ ",
		"   "
	};
	std::array<std::array<std::string, stringarrayResolution>, 10> numbersPointer = {number_zero, number_one, number_two, number_three, number_four, number_five, number_six, number_seven, number_eight, number_nine};
	std::array<std::string, stringarrayResolution> numbers(int number)
	{
		return numbersPointer[number];
	};
};

render::render() {
}

render::~render() {
}
