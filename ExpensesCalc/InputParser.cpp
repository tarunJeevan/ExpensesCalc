// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: https://pvs-studio.com

#include "InputParser.h"

InputParser::InputParser(std::string_view line)
{
	enum class States
	{
		ReadyForText,
		TextBegin
	} state = States::ReadyForText;

	std::stringstream ss;
	for (char c : line)
	{
		switch (state)
		{
		case States::ReadyForText:
			switch (c)
			{
			case ' ':
				break;
			default:
				ss << c;
				state = States::TextBegin;
				break;
			}
			break;
		case States::TextBegin:
			switch (c)
			{
			case ' ':
				m_args.emplace_back(ss.str());
				ss.str("");
				state = States::ReadyForText;
				break;
			default:
				ss << c;
				break;
			}
			break;
		}
	}

	if (state == States::TextBegin)
		m_args.emplace_back(ss.str());
}

InputParser InputParser::Subset(size_t offset, size_t count)
{
	InputParser subset;

	auto iter = m_args.begin() + offset;
	while (count-- && iter != m_args.end())
	{
		subset.m_args.emplace_back(*(iter++));
	}

	return subset;
}
