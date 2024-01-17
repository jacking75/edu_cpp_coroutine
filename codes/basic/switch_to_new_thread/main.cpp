#include "switch_to_new_thread.h"
#include <iostream>

Task foo()
{
	std::cout << "foo 1" << " " << std::this_thread::get_id() << std::endl;
	co_await switch_to_new_thread{};
	std::cout << "foo 2" << " " << std::this_thread::get_id() << std::endl;
}

int main()
{
	Task task = foo();
	std::cout << "\t\t main 1" << " " << std::this_thread::get_id() << std::endl;
	task.co_handler.resume();
	std::cout << "\t\t main 2" << " " << std::this_thread::get_id() << std::endl;

	int n = 0;
	std::cin >> n;
}