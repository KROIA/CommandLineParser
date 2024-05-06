#pragma once
#include "CommandLineParser_base.h"
#include <string>
#include <vector>

namespace CommandLineParser
{
	class COMMAND_LINE_PARSER_EXPORT Argument
	{
		public:
		Argument();
		Argument(const std::vector<std::string>& names, const std::string& value);
		Argument(const std::string& name, const std::string& value);
		Argument(const std::vector<std::string>& names, const std::vector<std::string>& value);
		Argument(const std::string& name, const std::vector<std::string>& value);
		Argument(const std::string& name);
		Argument(const std::vector<std::string>& names);
		
		Argument(const Argument& other);
		Argument& operator=(const Argument& other);
		
		~Argument();

		size_t getValueCount() const { return m_values.size(); }
		size_t getNameCount() const { return m_names.size(); }

		const std::vector<std::string>& getNames() const { return m_names; }
		const std::string& getName(size_t index = 0) const { return m_names[index]; }
		const std::string& getValue(size_t index = 0) const
		{			
			return m_values[index];
		}
		const std::vector<std::string>& getValues() const { return m_values; }


		bool hasName(const std::string& name) const;
		bool hasAnyName(const std::vector<std::string>& names) const;

		void setValue(const std::string& value) { m_values = { value }; }
		void setValues(const std::vector<std::string>& values) { m_values = values; }
		void clearValues() { m_values.clear(); }
		void addValue(const std::string& value) { m_values.push_back(value); }


		static void setArgumentPrefix(const std::string& prefix){ s_argumentPrefix = prefix; }
		static const std::string& getArgumentPrefix() { return s_argumentPrefix; }

		static void setArgumentAssignment(const std::string& assignment) { s_argumentAssignment = assignment; }
		static const std::string& getArgumentAssignment() { return s_argumentAssignment; }

		static void setValueListSeparator(const std::string& separator) { s_valueListSeparator = separator; }
		static const std::string& getValueListSeparator() { return s_valueListSeparator; }

		static bool parse(Argument& out, const std::string& command);
		
		

		private:
		std::vector<std::string> m_names;
		std::vector<std::string> m_values;

		static std::string s_argumentPrefix;
		static std::string s_argumentAssignment;
		static std::string s_valueListSeparator;
	};
}