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

	std::array<std::string, stringarrayResolution>* resultpointer = &result;

	std::array<std::string, stringarrayResolution>* renderTime(Time &currentTime)
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
		size_t splitTimeSize = splitTime.size();
		size_t i = 0;
		while(splitTimeSize>0) {
			for(size_t r = 0; r < resultSize; ++r)
			{
				(result[r]).append(" ");
			}
			for(size_t s = 0; s < resultSize; ++s)
			{
				(result[s]).append((*numbers(splitTime.front()))[s]);
			}
			if(i == (splitTimeSize-8) || i == (splitTimeSize-6) || i == (splitTimeSize-4))
			{
				for(size_t r = 0; r < resultSize; ++r)
				{
					(result[r]).append(number_space[r]);
				}
			}
			splitTime.pop_front();
			++i;
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
