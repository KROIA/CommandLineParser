#pragma once
#include "CommandLineParser_base.h"
#include "Argument.h"
#include <string>
#include <vector>

namespace CommandLineParser
{
	class COMMAND_LINE_PARSER_EXPORT ArgumentList : public std::vector<Argument>
	{
	public:
		bool parse(int argc, char* argv[]);
		bool parse(const std::string& commandLine);

		bool hasArgument(const std::string& name) const;
		bool hasArgument(const std::vector<std::string> &names) const; // returns true if any of the arguments is present
		size_t findArgument(const std::string& name) const;
		Argument getArgument(const std::string& name) const;
		bool getValueIfExists(const std::string& name, std::string& value) const;
		void shrink();
	private:

	};
}