#include <iostream>
#include <string>
#include "Header.h"

int main()
{
	Tester tester;
	std::string command = "";
	while (true)
	{
		std::cout << "Type exit, if you want to exit this application." << std::endl;
		std::cout << "Type scan, if you want to recursively scan current directory." << std::endl;
		std::cout << "Type scan path_to_the_directory, if you want to recursively scan directory." << std::endl;
		std::cout << "Type find file_name, if you want to find all possible paths to file_name." << std::endl;
		std::cout << std::endl;

		std::getline(std::cin, command);

		if (command == "exit")
		{
			system("pause");
			return 0;
		}
		else if (command == "scan")
		{
			tester.scan();
		}
		else if (command.substr(0, 5) == "scan ")
		{
			std::string buffer = command.substr(5, command.length() - 1);
			tester.scan(buffer);
		}
		else if (command.substr(0, 5) == "find ")
		{
			std::string buffer = command.substr(5, command.length() - 1);
			tester.find(buffer);
		}
		else
		{
			std::cout << "Unknown command!" << std::endl;
		}
		std::cout << std::endl;
	}

	system("pause");
	return 0;
}