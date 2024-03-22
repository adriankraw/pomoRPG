#include <array>
#include <string>
#include <list>
#include <iostream>

#define stringarrayResolution 8

class render {
public:
	render();
	render(render &&) = default;
	render(const render &) = default;
	render &operator=(render &&) = default;
	render &operator=(const render &) = default;
	~render();

	std::array<std::string, stringarrayResolution>* resultpointer = &result;

	std::array<std::string, stringarrayResolution>* renderTime(double currentTime)
	{
		//return a string that got modified into a time 
		std::list<int> splitTime;
		int time = (int)currentTime;
		int mili = (time%1000);
		int minutes = ((time/1000)/60);
		int seconds = ((((time/1000.0)/60.0)-minutes)*60);
		time = mili + (seconds*1000) + (minutes*100000);
		while (time >0)
		{
			splitTime.push_front(time % 10);
			time = (time /10);
		}
		for(int i = 0; i < result.size(); ++i)
		{
			result[i] = "";
		}
		int splitTimesize = splitTime.size();
		for (int i = 0; splitTime.size()>0; ++i) {
			for(int r = 0; r < result.size(); ++r)
			{
				(result[r]).append(" ");
			}
			for(int s = 0; s < result.size(); ++s)
			{
				(result[s]).append((*numbers(splitTime.front()))[s]);
			}
			if(i == (splitTimesize-6) || i == (splitTimesize-4))
			{
				for(int r = 0; r < result.size(); ++r)
				{
					(result[r]).append(number_space[r]);
				}
			}
			splitTime.pop_front();
		}
		return resultpointer;
	}

private:

	std::array<std::string, stringarrayResolution> result;
	
	std::array<std::string, stringarrayResolution> number_zero = {
		"xxxx",
		"x  x",
		"x  x",
		"x  x",
		"xxxx"
	};
	std::array<std::string, stringarrayResolution> number_one = {
		"   x",
		"   x",
		"   x",
		"   x",
		"   x"
	};
	std::array<std::string, stringarrayResolution> number_two = {
		"xxxx",
		"x  x",
		"  x ",
		" x  ",
		"xxxx"
	};
	std::array<std::string, stringarrayResolution> number_three = {
		"xxxx",
		"   x",
		" xxx",
		"   x",
		"xxxx"
	};
	std::array<std::string, stringarrayResolution> number_four = {
		"x  x",
		"x  x",
		"xxxx",
		"   x",
		"   x"
	};
	std::array<std::string, stringarrayResolution> number_five = {
		"xxxx",
		"x   ",
		"xxxx",
		"   x",
		"xxxx"
	};
	std::array<std::string, stringarrayResolution> number_six = {
		"xxxx",
		"x   ",
		"xxxx",
		"x  x",
		"xxxx"
	};
	std::array<std::string, stringarrayResolution> number_seven = {
		"xxxx",
		"x  x",
		"   x",
		"   x",
		"   x",
	};
	std::array<std::string, stringarrayResolution> number_eight = {
		"xxxx",
		"x  x",
		"xxxx",
		"x  x",
		"xxxx"
	};
	std::array<std::string, stringarrayResolution> number_nine = {
		"xxxx",
		"x  x",
		"xxxx",
		"   x",
		"xxxx"
	};
	std::array<std::string, stringarrayResolution> number_space = {
		"   ",
		" x ",
		"   ",
		" x ",
		"   "
	};
	std::array<std::array<std::string, stringarrayResolution>*, 10> numbersPointer = {&number_zero, &number_one, &number_two, &number_three, &number_four, &number_five, &number_six, &number_seven, &number_eight, &number_nine};
	std::array<std::string, stringarrayResolution>* numbers(int number)
	{
		return numbersPointer[number];
	};
};

render::render() {
}

render::~render() {
}
