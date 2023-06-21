// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <ostream>
#include <iomanip>
#include <algorithm>

class ExpSheet
{
public:
	struct Entry
	{
		double value = 0.0;
		std::string label;

		inline bool operator ==(const Entry& rhs) const
		{
			return label == rhs.label;
		}
	};

public:
	ExpSheet() = default;
	ExpSheet(const ExpSheet&) = default;

	ExpSheet& operator =(const ExpSheet&) = default;

	bool Add(std::string_view label, double val);
	bool Del(std::string_view label);
	void List(std::ostream& os) const;
	double Eval() const;

private:
	std::vector<Entry> m_entries;
};