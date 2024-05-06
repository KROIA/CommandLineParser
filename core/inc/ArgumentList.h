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
		size_t findArgument(const std::string& name) const;
		Argument getArgument(const std::string& name) const;
		void shrink();
	private:

	};
}