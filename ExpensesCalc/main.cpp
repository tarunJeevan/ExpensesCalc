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
		
		// Parse user input
		std::string cmd = input[0];
		std::transform(cmd.begin(), cmd.end(), cmd.begin(), [](char c) { return std::tolower(c); });
		auto args = input.Subset(1);
		
		// Exit the program
		if (cmd == "exit") 
		{
			// Check if current expense sheet has been saved
			if (!expenses.isSaved())
			{
				char confirmation;
				std::cout << "Do you wish to save your changes? (y/n)" << std::endl;
				std::cin >> confirmation;
				std::filesystem::path name;

				switch (std::tolower(confirmation))
				{
					// User wants to save changes
					case 'y':
						std::cout << "Please enter file name: ";
						std::cin >> name;
						
						if (!expenses.Save(name))
						{
							Complain(repl, "Failed to save file! Try: save <PATH>");
						}
						else
						{
							Success(repl);
							return 0;
						}
							
						break;
					// User doesn't want to save changes
					case 'n':
						Success(repl);
						return 0;
						break;
					// Invalid response
					default:
						std::cout << "Invalid response" << std::endl;
				}
			}
			else
			{
				Success(repl);
				return 0;
			}
		}
		// Add income
		else if (cmd == "add") 
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
		// Add an expense
		else if (cmd == "exp") 
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
		// Delete an expense or income
		else if (cmd == "del") 
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
		// List all income and expenses
		else if (cmd == "list") 
		{
			expenses.List(repl, std::cout);
		}
		// Evaluate expense sheet and print balance
		else if (cmd == "eval") 
		{
			auto total = expenses.Eval();
			if (repl)
				std::cout << total << std::endl;
			else
				std::cout << "Total: " << std::fixed << std::setprecision(2) << total << std::endl;
		}
		// Open an expense sheet
		else if (cmd == "open") 
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
		// Save current expense sheet
		else if (cmd == "save") 
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
		// Empty current expense sheet
		else if (cmd == "clear") 
		{
			expenses.Clear();
			Success(repl);
		}
		// Export current expense sheet
		else if (cmd == "export") 
		{
			if (args.Count() == 2)
			{
				if (args[0] == "csv") // Export to CSV
				{
					if (expenses.ExportCSV(args[1]))
						Success(repl);
					else
						std::cout << "Export failed!" << std::endl;
				}
				else if (args[0] == "html") // Export to HTML
				{
					if (expenses.ExportHTML(args[1]))
						Success(repl);
					else
						std::cout << "Export failed!" << std::endl;
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
		// List all commands and their functions
		else if (cmd == "help") 
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
		// Invalid command
		else
		{
			Complain(repl, "Command is invalid");
		}
		
	}
}