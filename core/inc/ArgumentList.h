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
		static bool parse(ArgumentList& arguments, int argc, char* argv[]);
		static bool parse(ArgumentList& arguments, const std::string& commandLine);

		bool hasArgument(const std::string& name) const;
		size_t findArgument(const std::string& name) const;
		void shrink();
	private:

	};
}