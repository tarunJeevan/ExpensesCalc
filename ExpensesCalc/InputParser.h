// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <string_view>

class InputParser
{
public:
	InputParser() = default;
	InputParser(const InputParser&) = default;
	InputParser(std::string_view line);

	InputParser& operator = (const InputParser&) = default;

	InputParser Subset(size_t offset, size_t count = std::numeric_limits<size_t>::max());

	inline size_t Count() const { return m_args.size(); }
	inline const std::string& Get(size_t index) const { return m_args[index]; }
	inline const std::string& operator [](size_t index) const { return Get(index); }


private:
	std::vector<std::string> m_args;
};