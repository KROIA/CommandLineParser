#include "Argument.h"
#include <regex>

namespace CommandLineParser
{
	std::string Argument::s_argumentPrefix = "-";
	std::string Argument::s_argumentAssignment = "=";

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

	bool Argument::parse(std::vector<Argument>& arguments, 
						 const std::string& argument)
	{
		bool success = true;
		std::string arg = argument;
		// Regexp to split arguments:
			// (-([a-zA-Z]+[a-zA-Z0-9]*)(?:=((?:[^"\s]+(;)*|"(?:[^"\\]|\\.)*")*))?)
		const std::string argumentRegex = "("+s_argumentPrefix+"([a-zA-Z]+[a-zA-Z0-9]*)(?:"+s_argumentAssignment+"((?:[^\"\\s]+(;)*|\"(?:[^\"\\\\]|\\\\.)*\")*))?)";
		std::regex re(argumentRegex);
		std::smatch match;
		std::vector<std::pair<std::string, std::string>> keyValues;
		while (std::regex_search(arg, match, re))
		{
			keyValues.push_back(std::make_pair(match[2].str(), match[3].str()));
			arg = match.suffix().str();
		}

		for (const auto& kv : keyValues)
		{
			std::string key = kv.first;
			std::string value = kv.second;
			if (key.empty())
			{
				success = false;
				continue;
			}
			if (value.empty())
			{
				arguments.push_back(Argument(key));
				continue;
			}
			std::vector<std::string> values;
			bool inQuote = false;
			std::string current;
			size_t start = 0, end = 0;
			for (size_t i = 0; i < value.size(); ++i)
			{
				if (value[i] == '"')
				{
					bool innerQuote = false;
					if (i > 0)
						if (value[i - 1] == '\\')
							innerQuote = true;
					if (!innerQuote)
					{
						inQuote = !inQuote;
						if(inQuote)
							start = i+1;
						else
						{
							end = i;
						//	current = value.substr(start, end - start);
						//	values.push_back(current);
						}
						continue;
					}
				}
				if (value[i] == ';' && !inQuote)
				{
					end = i;
					current = value.substr(start, end - start);
					values.push_back(current);
					start = end + 1;
					//current.clear();
					continue;
				}
				//current += value[i];
			}
			Argument argInst(key, values);
			arguments.push_back(argInst);
		}

		shrink(arguments);

		return success;
	}
	void Argument::shrink(std::vector<Argument>& arguments)
	{
		// remove duplicates and empty arguments
		std::vector<Argument> result;
		result.reserve(arguments.size());
		for (const Argument& arg : arguments)
		{
			if (arg.getNames().empty() || arg.getName().empty())
				continue;
			if (arg.getValues().empty())
				continue;

			bool found = false;
			for (const Argument& res : result)
			{
				if (res.hasAnyName(arg.getNames()))
				{
					found = true;
					break;
				}
			}
			if (!found)
				result.push_back(arg);
		}
		arguments = result;
	}

}