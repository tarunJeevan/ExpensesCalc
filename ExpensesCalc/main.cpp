// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include <iostream>
#include <algorithm>
#include "InputParser.h"

int main()
{
	std::string line;

	// Create enum to handle different commands?
	
	while (true)
	{
		// Get user input
		std::cout << "> ";
		std::getline(std::cin, line);
		InputParser input(line);
		
		std::string cmd = input[0];
		std::transform(cmd.begin(), cmd.end(), cmd.begin(), [](char c) { return std::tolower(c); });
		auto args = input.Subset(1);
		
		// Exit if exit command is used
		if (cmd == "exit")
			return 0;
		else if (cmd == "add")
			std::cout << "ADD" << std::endl;
		else if (cmd == "del")
			std::cout << "DELETE" << std::endl;
		else if (cmd == "exp")
			std::cout << "ADD EXPENSE" << std::endl;
		else if (cmd == "list")
			std::cout << "LIST" << std::endl;
		else if (cmd == "eval")
			std::cout << "EVALUATE" << std::endl;
		else 
			std::cout << "Command \'" << cmd << "\' invalid" << std::endl;

		
	}
}