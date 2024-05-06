#include "Argument.h"
#include <regex>

namespace CommandLineParser
{
	std::string Argument::s_argumentPrefix = "-";
	std::string Argument::s_argumentAssignment = "=";
	std::string Argument::s_valueListSeparator = ";";

	Argument::Argument(const std::vector<std::string>& names, const std::string& value)
		: m_names(names)
		, m_values({ value })
	{
	}

	Argument::Argument(const std::string& name, const std::string& value)
		: m_names({ name })
		, m_values({ value })
	{
	}

	Argument::Argument(const std::vector<std::string>& names, const std::vector<std::string>& values)
		: m_names(names)
		, m_values(values)
	{
	}

	Argument::Argument(const std::string& name, const std::vector<std::string>& values)
		: m_names({ name })
		, m_values(values)
	{
		if (m_values.empty())
			m_values.push_back("");
	}

	Argument::Argument(const std::string& name)
		: m_names({ name })
		, m_values({ "" })
	{
	}

	Argument::Argument(const std::vector<std::string>& names)
		: m_names(names)
		, m_values({ "" })
	{
	}

	Argument::Argument(const Argument& other)
		: m_names(other.m_names)
		, m_values(other.m_values)
	{
	}

	Argument& Argument::operator=(const Argument& other)
	{
		m_names = other.m_names;
		m_values = other.m_values;
		return *this;
	}

	Argument::~Argument()
	{
	}

	bool Argument::hasName(const std::string& name) const
	{
		for (const std::string& n : m_names)
		{
			if (n == name)
				return true;
		}
		return false;
	}
	bool Argument::hasAnyName(const std::vector<std::string>& names) const
	{
		for (const std::string& n : names)
		{
			if (hasName(n))
				return true;
		}
		return false;
	}

	
	bool Argument::parse(Argument& out, const std::string& command)
	{
		const std::string argumentRegex = "("+s_argumentPrefix+"([a-zA-Z]+[a-zA-Z0-9]*)(?:" + s_argumentAssignment + "(.*)|))";

		std::regex re(argumentRegex);
		std::smatch match;
		if (std::regex_search(command, match, re))
		{
			std::string key = match[2].str();
			std::string value = match[3].str();
			if (key.empty())
				return false;
			if (value.empty())
			{
				out = Argument(key);
				return true;
			}
			
			std::vector<std::string> values;
			size_t firstArraySeparator;
			while ((firstArraySeparator = value.find(s_valueListSeparator)) != std::string::npos)
			{
				std::string substr = value.substr(0, firstArraySeparator);
				if (substr.size() > 0)
					values.push_back(substr);
				value = value.substr(firstArraySeparator + s_valueListSeparator.size());
			}
			if(value.size() > 0)
				values.push_back(value);

			out = Argument(key, values);
			return true;
		}
		return false;
	}


	

}