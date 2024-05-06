#include "ArgumentList.h"
#include <regex>


namespace CommandLineParser
{
	bool ArgumentList::parse(int argc, char* argv[])
	{
		std::vector<Argument> result;
		result.reserve(argc - 1);
		std::string currentArg;
		bool copyPrevious = false;
		for (int i = argc - 1; i >= 1; --i)
		{
			Argument arg("");
			if (copyPrevious)
			{
				currentArg = std::string(argv[i]) + " " + currentArg;
			}
			else
				currentArg = argv[i];
			if (Argument::parse(arg, currentArg))
			{
				result.push_back(arg);
				copyPrevious = false;
			}
			else
			{
				copyPrevious = true;
				continue;
			}
		}

		vector::clear();
		vector::reserve(result.size());
		for (size_t i = 0; i < result.size(); ++i)
		{
			vector::push_back(result[result.size() - i - 1]);
		}
		shrink();
		return true;
	}
	bool ArgumentList::parse(const std::string& commandLine)
	{
		bool success = true;
		std::string arg = commandLine;
		// Regexp to split arguments:
			// (-([a-zA-Z]+[a-zA-Z0-9]*)(?:=((?:[^"\s]+(;)*|"(?:[^"\\]|\\.)*")*))?)
		const std::string argumentRegex = "(" + Argument::getArgumentPrefix() + "([a-zA-Z]+[a-zA-Z0-9]*)(?:" + Argument::getArgumentAssignment() + "((?:[^\"\\s]+(;)*|\"(?:[^\"\\\\]|\\\\.)*\")*))?)";
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
				vector::push_back(Argument(key));
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
						if (inQuote)
							start = i + 1;
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
			vector::push_back(argInst);
		}

		shrink();
		if(!success)
			vector::clear();
		return success;
	}
	bool ArgumentList::hasArgument(const std::string& name) const
	{
		for (const Argument& arg : *this)
		{
			if (arg.hasName(name))
				return true;
		}
		return false;
	}
	bool ArgumentList::hasArgument(const std::vector<std::string>& names) const
	{
		for (const Argument& arg : *this)
		{
			if (arg.hasAnyName(names))
				return true;
		}
		return false;
	}
	void ArgumentList::shrink()
	{
		// remove duplicates and empty arguments
		ArgumentList result;
		result.reserve(vector::size());
		for (const Argument& arg : *this)
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
		this->operator=(result);
	}
	size_t ArgumentList::findArgument(const std::string& name) const
	{
		for (size_t i = 0; i < vector::size(); ++i)
		{
			if ((*this)[i].hasName(name))
				return i;
		}
		return std::string::npos;
	}
	Argument ArgumentList::getArgument(const std::string& name) const
	{
		size_t index = findArgument(name);
		if (index == std::string::npos)
			return Argument("");
		return (*this)[index];
	}
	bool ArgumentList::getValueIfExists(const std::string& name, std::string& value) const
	{
		size_t index = findArgument(name);
		if (index == std::string::npos)
			return false;
		value = (*this)[index].getValue();
		return true;
	}
}