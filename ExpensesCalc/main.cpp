// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include <iostream>
#include "ExpSheet.h"
#include "InputParser.h"

void Complain(bool repl, const std::string& msg)
{
	if (repl)
		std::cout << 0 << std::endl;
	else
		std::cout << msg << std::endl;
}

void Success(bool repl)
{
	if (repl)
		std::cout << 1 << std::endl;
}

int main(int argc, char** argv)
{
	ExpSheet expenses;

	// Enable REPL mode
	bool repl = false;

	// Check if user wants to open a specific expense sheet
	for (int i = 1; i < argc; ++i)
	{
		if (argv[i][0] != '-')
		{
			std::filesystem::path path = argv[1];
			expenses.Open(path);
		}
		else
		{
			std::string sarg = &argv[i][1];
			if (sarg == "repl")
				repl = true;
		}
	}

	std::string line;

	// Create enum to handle different commands?
	
	while (true)
	{
		if (!repl)
			std::cout << "> "; // Get user input
		
		std::getline(std::cin, line);
		InputParser input(line);
		
		std::string cmd = input[0];
		std::transform(cmd.begin(), cmd.end(), cmd.begin(), [](char c) { return std::tolower(c); });
		auto args = input.Subset(1);
		
		if (cmd == "exit") // Exit the program
		{
			Success(repl);
			return 0;
		}
		else if (cmd == "add") // Add income
		{
			if (args.Count() == 2)
			{
				const auto& label = args[0];
				double value = atof(args[1].c_str());

				if (!expenses.Add(label, value))
					Complain(repl, "Failed to add!");
				else
					Success(repl);
			}
			else
				Complain(repl, "Usage: add <LABEL> <VALUE>");
		}
		else if (cmd == "exp") // Add an expense
		{
			if (args.Count() == 2)
			{
				const auto& label = args[0];
				double value = atof(args[1].c_str()) * -1.0;

				if (!expenses.Add(label, value))
					Complain(repl, "Failed to add!");
				else
					Success(repl);
			}
			else
				Complain(repl, "Usage: exp <LABEL> <VALUE>");
		}
		else if (cmd == "del") // Delete an expense or income
		{
			if (args.Count() == 1)
			{
				if (!expenses.Del(args[0]))
					Complain(repl, "Failed to delete!");
				else
					Success(repl);
			}
			else
				Complain(repl, "Usage: del <LABEL>");
		}
		else if (cmd == "list") // List all income and expenses
		{
			expenses.List(repl, std::cout);
		}
		else if (cmd == "eval") // Evaluate expense sheet and print balance
		{
			auto total = expenses.Eval();
			if (repl)
				std::cout << total << std::endl;
			else
				std::cout << "Total: " << std::fixed << std::setprecision(2) << total << std::endl;
		}
		else if (cmd == "open") // Open an expense sheet
		{
			if (args.Count() == 1)
			{
				std::filesystem::path p = args[0];

				if (!expenses.Open(p))
					Complain(repl, "Failed to open file!");
				else
					Success(repl);
			}
			else
				Complain(repl, "Usage: open <PATH>");
		}
		else if (cmd == "save") // Save current expense sheet
		{
			if (args.Count() == 1)
			{
				std::filesystem::path p = args[0];

				if (!expenses.Save(p))
					Complain(repl, "Failed to save file!");
				else
					Success(repl);
			}
			else if (args.Count() == 0)
			{
				if (!expenses.Save())
					Complain(repl, "Failed to save file! Try: save <PATH>");
				else
					Success(repl);
			}
			else
				Complain(repl, "Usage: save <PATH>");
		}
		else if (cmd == "clear") // Empty current expense sheet
		{
			expenses.Clear();
			Success(repl);
		}
		else if (cmd == "export") // Export current expense sheet
		{
			if (args.Count() == 2)
			{
				if (args[0] == "csv") // Export to CSV
				{
					if (expenses.ExportCSV(args[1]))
						Success(repl);
					else
						Success(repl);
				}
				else if (args[0] == "html") // Export to HTML
				{
					if (expenses.ExportHTML(args[1]))
						Success(repl);
					else
						Success(repl);
				}
				else
				{
					Complain(repl, "Usage: export <CSV/HTML> <FILE>");
				}
			}
			else
			{
				Complain(repl, "Usage: export <CSV/HTML> <FILE>");
			}
		}
		else if (cmd == "help") // List all commands and their functions
		{
			std::cout << "\nUsage: CMD [args...]\n\n"
					"open <PATH> \t\t\t(Open a saved expense sheet)\n"
					"save <FILE> \t\t\t(Save current expense sheet as FILE)\n"
					"export <CSV/HTML> <FILE> \t(Export current expense sheet as CSV/HTML file named FILE)\n"
					"add <LABEL> <VALUE> \t\t(Add income named LABEL with amount VALUE)\n"
					"exp <LABEL> <VALUE> \t\t(Add expense named LABEL with amount VALUE)\n"
					"del <LABEL> \t\t\t(Delete income or expense named LABEL)\n"
					"clear \t\t\t\t(Empty current expense sheet)\n"
					"list \t\t\t\t(List all income and expenses in expense sheet)\n"
					"eval \t\t\t\t(Evaluate expense sheet and print balance)\n"
					"exit \t\t\t\t(Exit the application)\n";
		}
		else
		{
			Complain(repl, "Command is invalid");
		}
		
	}
}