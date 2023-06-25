// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "ExpSheet.h"

void ExpSheet::Entry::Serialize(std::ostream& out) const
{
	out.write(label.c_str(), label.length() + 1);
	out.write((char*)&value, sizeof(double));
}

void ExpSheet::Entry::Deserialize(std::istream& in)
{
	std::stringstream ss;
	char c;

	do
	{
		in.read(&c, 1);
		if (c != '\0')
			ss << c;

	} while (c != '\0');

	label = ss.str();
	in.read((char*)&value, sizeof(double));
}

bool ExpSheet::Open(const std::filesystem::path& dataFile)
{
	auto path = dataFile;
	path.replace_extension(".pexc");

	std::ifstream fileIn(path, std::ios::in | std::ios::binary);

	if (fileIn.is_open())
	{
		m_path = path;

		size_t numElements = 0;
		fileIn.read((char*)&numElements, sizeof(size_t));
		m_entries.clear();

		for (size_t i = 0; i < numElements; i++)
		{
			Entry e;
			e.Deserialize(fileIn);
			m_entries.emplace_back(std::move(e));
		}

		return true;
	}
	return false;
}

bool ExpSheet::Save(const std::filesystem::path& dataFile) const
{
	auto path = dataFile;

	if (dataFile.empty())
		if (m_path.empty())
			return false;
		else
			path = m_path;

	path.replace_extension(".pexc");

	std::ofstream fileOut(path, std::ios::out | std::ios::trunc | std::ios::binary);

	if (fileOut.is_open())
	{
		size_t numElements = m_entries.size(); // Dumb way of doing this. Research better way
		fileOut.write((char*)&numElements, sizeof(size_t));

		for (const Entry& e : m_entries)
		{
			e.Serialize(fileOut);
		}
		return true;
	}
	return false;
}

void ExpSheet::Clear()
{
	m_path = "";
	m_entries.clear();
}

bool ExpSheet::Add(std::string_view label, double val)
{
	Entry e;
	e.label = label;
	e.value = val;

	bool canInsert = std::find(m_entries.begin(), m_entries.end(), e) == m_entries.end();

	if (canInsert)
		m_entries.emplace_back(std::move(e));

	return canInsert;
}

bool ExpSheet::Del(std::string_view label)
{
	Entry e;
	e.label = label;
	auto el = std::find(m_entries.begin(), m_entries.end(), e);

	if (el != m_entries.end())
	{
		m_entries.erase(el);
		return true;
	}

	return false;
}

void ExpSheet::List(bool repl, std::ostream& os) const
{
	os << std::fixed << std::setprecision(2);

	size_t index = 0;
	for (const Entry& e : m_entries)
	{
		if (repl)
			os << ++index << "," << e.label << "," << e.value << "\n";
		else
			os << "(" << index << ") " << e.label << " -> " << e.value << "\n";
	}

	if (repl && index == 0)
		os << 0 << "\n";
}

double ExpSheet::Eval() const
{
	double total = 0.0;

	for (const Entry& e : m_entries)
		total += e.value;

	return total;
}

bool ExpSheet::ExportCSV(const std::filesystem::path& file) const
{
	auto xpath = file;
	xpath.replace_extension(".csv");

	auto path = xpath;
	if (!path.remove_filename().empty())
		std::filesystem::create_directories(path);

	std::ofstream FileOut(xpath, std::ios::out | std::ios::trunc);

	if (FileOut.is_open())
	{
		FileOut << "Pos,Expense,Value" << "\n";
		size_t i = 0;

		for (const Entry& e : m_entries)
		{
			FileOut << ++i << "," << e.label << "," << e.value << "\n";
		}
		FileOut << "Total" << "," << "," << Eval() << "\n";

		return true;
	}
	return false;
}

bool ExpSheet::ExportHTML(const std::filesystem::path& file) const
{
	auto xpath = file;
	xpath.replace_extension(".html");

	auto path = xpath;
	if (!path.remove_filename().empty())
		std::filesystem::create_directories(path);

	std::ofstream FileOut(xpath, std::ios::out | std::ios::trunc);

	if (FileOut.is_open())
	{
		FileOut <<
			R"(
			<!doctype html>
			<html lang="en" data-bs-theme="dark">
				<head>
					<meta charset="utf-8">
					<meta name="viewport" content="width=device-width, initial-scale=1">
					<title>Expense Sheet</title>
					<link href="https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/css/bootstrap.min.css" rel="stylesheet" integrity="sha384-9ndCyUaIbzAi2FUVXJi0CjmCapSmO7SnpJef0486qhLnuZ2cdeRhO02iuK6FUUVM" crossorigin="anonymous">
				</head>
				<body>
					<nav class="navbar bg-body-tertiary">
						<div class="container-fluid">
						<span class="navbar-brand mb-0 h1"> Expense Sheet: )" << m_path.filename().string() << R"(</span>
						</div>
					</nav>
					<div class="container mt-2">
						<div class="card">
							<div class="card-body">
								<table class="table table-hover">
									<thead>
										<tr>
											<th scope="col">#</th>
											<th scope="col">Entry</th>
											<th scope="col">Value</th>
										</tr>
									</thead>
									<tbody>
		)";

		size_t index = 0;
		for (const Entry& e : m_entries)
		{
			FileOut << "<tr><th scope=\"row\">" << ++index << "</th>" 
				<< "<td>" << e.label << "</td>"
				<< "<td class=\"" << TextHTMLClass(e.value) << "\">" << e.value << "</td></tr>";
		}

		FileOut << 
			R"(
									</tbody>
								</table>
								<p class="fs-3 )" << TextHTMLClass(Eval()) << R"(">TOTAL: $)" << Eval() << R"(</p>
							</div>
						</div>
					</div>
					<script src = "https://cdn.jsdelivr.net/npm/bootstrap@5.3.0/dist/js/bootstrap.bundle.min.js" integrity = "sha384-geWF76RCwLtnZ8qwWowPQNguL3RmwHVBC9FhGdlKrxdiJJigb/j/68SIy3Te4Bkz" crossorigin = "anonymous">< / script>
				</body>
			</html>
		)";

		return true;
	}
	return false;
}
