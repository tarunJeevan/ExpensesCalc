// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include <iostream>
#include "ExpSheet.h"
#include "InputParser.h"

int main()
{
	std::string line;
	ExpSheet expenses;

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
		
		if (cmd == "exit") // Exit the program
		{
			return 0;
		}
		else if (cmd == "add") // Add revenue
		{
			if (args.Count() == 2)
			{
				const auto& label = args[0];
				double value = atof(args[1].c_str());

				if (!expenses.Add(label, value))
					std::cout << "Failed to add!" << std::endl;
			}
			else
				std::cout << "Usage: add <label> <value>" << std::endl;
		}
		else if (cmd == "del") // Delete an expense or revenue
		{
			if (args.Count() == 1)
			{
				if (!expenses.Del(args[0]))
					std::cout << "Failed to delete!" << std::endl;
			}
			else
				std::cout << "Usage: del <label>" << std::endl;
		}
		else if (cmd == "exp") // Add an expense
		{
			if (args.Count() == 2)
			{
				const auto& label = args[0];
				double value = atof(args[1].c_str()) * -1.0;

				if (!expenses.Add(label, value))
					std::cout << "Failed to add!" << std::endl;
			}
			else
				std::cout << "Usage: exp <label> <value>" << std::endl;
		}
		else if (cmd == "list") // List all revenue and expenses
		{
			expenses.List(std::cout);
		}
		else if (cmd == "eval")
		{
			//std::cout << "EVALUATE" << std::endl;
			auto total = expenses.Eval();
			std::cout << "Total: " << std::fixed << std::setprecision(2) << total << std::endl;
		}
		else
		{
			std::cout << "Command \'" << cmd << "\' invalid" << std::endl;
		}
		
	}
}