// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <ostream>
#include <istream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <filesystem>
#include <algorithm>

class ExpSheet
{
public:
	struct Entry
	{
		// Binary data format -> byte[...]\0byte[sizeof(double)]

		double value = 0.0;
		std::string label;

		void Serialize(std::ostream& out) const;
		void Deserialize(std::istream& in);

		inline bool operator ==(const Entry& rhs) const
		{
			return label == rhs.label;
		}
	};

public:
	ExpSheet() = default;
	ExpSheet(const ExpSheet&) = default;

	ExpSheet& operator =(const ExpSheet&) = default;

	bool Open(const std::filesystem::path& dataFile);
	bool Save(const std::filesystem::path& dataFile) const;

	bool Add(std::string_view label, double val);
	bool Del(std::string_view label);
	void List(std::ostream& os) const;
	double Eval() const;

private:
	std::vector<Entry> m_entries;
};