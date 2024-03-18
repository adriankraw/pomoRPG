#include <array>
#include <string>
#include <list>
#include <iostream>


class render {
public:
	render();
	render(render &&) = default;
	render(const render &) = default;
	render &operator=(render &&) = default;
	render &operator=(const render &) = default;
	~render();

	std::array<std::string, 8>* resultpointer = &result;

	std::array<std::string, 8>* renderTime(double currentTime)
	{
		//return a string that got modified into a time 
		std::list<int> splitTime;
		int time = (int)currentTime;
		while (time >0)
		{
			splitTime.push_front(time % 10);
			time = (time /10);
		}
		for(int i = 0; i < result.size(); ++i)
		{
			result[i] = "";
		}
		for (int i = 0; splitTime.size()>0; ++i) {
			for(int r = 0; r < result.size(); ++r)
			{
				result[r].append(" ");
			}
			for(int s = 0; s < result.size(); ++s)
			{
				(result[s]).append((*numbers(splitTime.front()))[s]);
			}
			splitTime.pop_front();
			
		}
		return resultpointer;
	}

private:

	std::array<std::string, 8> result;
	
	std::array<std::string, 8> number_zero = {
		"xxxx",
		"x  x",
		"x  x",
		"x  x",
		"x  x",
		"x  x",
		"x  x",
		"xxxx"
	};
	std::array<std::string, 8> number_one = {
		"   x",
		"   x",
		"   x",
		"   x",
		"   x",
		"   x",
		"   x",
		"   x"
	};
	std::array<std::string, 8> number_two = {
		"xxxx",
		"x  x",
		"   x",
		"  x ",
		" x  ",
		"x   ",
		"x  x",
		"xxxx"
	};
	std::array<std::string, 8> number_three = {
		"xxxx",
		"x  x",
		"   x",
		" xxx",
		" xxx",
		"   x",
		"x  x",
		"xxxx"
	};
	std::array<std::string, 8> number_four = {
		"x  x",
		"x  x",
		"x  x",
		"xxxx",
		"   x",
		"   x",
		"   x",
		"   x"
	};
	std::array<std::string, 8> number_five = {
		"xxxx",
		"x   ",
		"x   ",
		"xxxx",
		"   x",
		"   x",
		"x  x",
		"xxxx"
	};
	std::array<std::string, 8> number_six = {
		"xxxx",
		"x  x",
		"x   ",
		"x   ",
		"xxxx",
		"x  x",
		"x  x",
		"xxxx"
	};
	std::array<std::string, 8> number_seven = {
		"xxxx",
		"x  x",
		"   x",
		"   x",
		"   x",
		"   x",
		"   x",
		"   x"
	};
	std::array<std::string, 8> number_eight = {
		"xxxx",
		"x  x",
		"x  x",
		"xxxx",
		"xxxx",
		"x  x",
		"x  x",
		"xxxx"
	};
	std::array<std::string, 8> number_nine = {
		"xxxx",
		"x  x",
		"x  x",
		"xxxx",
		"   x",
		"   x",
		"x  x",
		"xxxx"
	};
	std::array<std::string, 8> number_space = {
		"   ",
		"   ",
		" x ",
		"   ",
		"   ",
		" x ",
		"   ",
		"   "
	};
	std::array<std::array<std::string, 8>*, 10> numbersPointer = {&number_zero, &number_one, &number_two, &number_three, &number_four, &number_five, &number_six, &number_seven, &number_eight, &number_nine};
	std::array<std::string, 8>* numbers(int number)
	{
		return numbersPointer[number];
	};
};

render::render() {
}

render::~render() {
}
