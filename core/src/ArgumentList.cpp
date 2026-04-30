#include "ArgumentList.h"
#include <regex>
#include <cctype>


namespace CommandLineParser
{
	static std::vector<std::string> splitValueList(const std::string& value, const std::string& sep)
	{
		std::vector<std::string> values;
		if (sep.empty())
		{
			if (!value.empty())
				values.push_back(value);
			return values;
		}
		size_t start = 0;
		while (start <= value.size())
		{
			size_t pos = value.find(sep, start);
			if (pos == std::string::npos)
			{
				if (start < value.size())
					values.push_back(value.substr(start));
				break;
			}
			if (pos > start)
				values.push_back(value.substr(start, pos - start));
			start = pos + sep.size();
		}
		return values;
	}

	bool ArgumentList::parse(int argc, char* argv[])
	{
		vector::clear();
		if (argc <= 1)
			return true;
		vector::reserve(argc - 1);

		const std::string& prefix = Argument::getArgumentPrefix();
		const std::string& assign = Argument::getArgumentAssignment();
		const std::string& sep = Argument::getValueListSeparator();

		auto startsWithPrefix = [&prefix](const std::string& s) -> bool
		{
			return !prefix.empty() && s.size() >= prefix.size()
				&& s.compare(0, prefix.size(), prefix) == 0;
		};

		for (int i = 1; i < argc; ++i)
		{
			std::string token = argv[i];
			if (!startsWithPrefix(token))
				continue;
			token.erase(0, prefix.size());
			// Tolerate "--name" when the configured prefix is a single "-".
			while (!token.empty() && token.compare(0, prefix.size(), prefix) == 0)
				token.erase(0, prefix.size());

			// Name must start with a letter, then [a-zA-Z0-9_-]*.
			if (token.empty() || !std::isalpha(static_cast<unsigned char>(token[0])))
				continue;
			size_t nameEnd = 1;
			while (nameEnd < token.size())
			{
				char c = token[nameEnd];
				if (std::isalnum(static_cast<unsigned char>(c)) || c == '_' || c == '-')
					++nameEnd;
				else
					break;
			}
			std::string name = token.substr(0, nameEnd);

			std::string value;
			bool hasValue = false;
			if (nameEnd < token.size())
			{
				// The only valid char after the name is the assignment delimiter.
				if (assign.empty() || token.compare(nameEnd, assign.size(), assign) != 0)
					continue;
				value = token.substr(nameEnd + assign.size());
				hasValue = true;
			}
			else if (i + 1 < argc && !startsWithPrefix(argv[i + 1]))
			{
				value = argv[++i];
				hasValue = true;
			}

			if (!hasValue || value.empty())
				vector::push_back(Argument(name));
			else
				vector::push_back(Argument(name, splitValueList(value, sep)));
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
		const std::string argumentRegex = "(" + Argument::getArgumentPrefix() + "([a-zA-Z][a-zA-Z0-9_-]*)(?:" + Argument::getArgumentAssignment() + "((?:[^\"\\s]+|\"(?:[^\"\\\\]|\\\\.)*\")*))?)";
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
			const std::string& sep = Argument::getValueListSeparator();
			for (size_t i = 0; i < value.size(); ++i)
			{
				char c = value[i];
				if (c == '\\' && i + 1 < value.size())
				{
					current += value[i + 1];
					++i;
					continue;
				}
				if (c == '"')
				{
					inQuote = !inQuote;
					continue;
				}
				if (!inQuote && !sep.empty() && value.compare(i, sep.size(), sep) == 0)
				{
					if (!current.empty())
						values.push_back(current);
					current.clear();
					i += sep.size() - 1;
					continue;
				}
				current += c;
			}
			if (!current.empty())
				values.push_back(current);
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