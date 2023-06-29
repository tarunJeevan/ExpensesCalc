# ExpensesCalc

A customized expenses calculator since Excel isn't for me. It'll start off pretty basic, but I plan on building it into a fully functioning application. 

**User Input:**
- CMD \[args...\]

**CMD(s):**
- exp (add expenses)
- add (add revenue)
- del (remove expenses or revenue)
- list (list all expenses and revenue)
- eval (evaluate expenses and list balance)
- clear (clear the opened expense sheet)
- exit (exit the application)
- save (saves current expense sheet)
- open (opens a saved expense sheet)
- export (export current expense sheet as CSV or HTML file)
- help (list all commands and their functions)

**Todo:**
- Add time stamps to entries
- Add auto-fill to UI
- Optimize REPL mode command line switch

**Log:**
- Added desired basic functionality (06/21/2023)
- Added basic serialization (06/23/2023)
- Created release build with basic serialization (06/24/2023)
- Added CSV and HTML file export options (06/24/2023)
- Added help command to list all options (06/25/2023)
- Added basic REPL functionality (06/25/2023)
- Added save warning if current expense sheet hasn't been saved (06/29/2023)