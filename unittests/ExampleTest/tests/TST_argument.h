#pragma once

#include "UnitTest.h"
#include "CommandLineParser.h"

class TST_argument : public UnitTest::Test
{
	TEST_CLASS(TST_argument)
public:
	TST_argument()
		: Test("TST_argument")
	{
		ADD_TEST(TST_argument::flagShort);
		ADD_TEST(TST_argument::flagLong);
		ADD_TEST(TST_argument::keyValue);
		ADD_TEST(TST_argument::hyphenInName);
		ADD_TEST(TST_argument::underscoreInName);
		ADD_TEST(TST_argument::valueList);
		ADD_TEST(TST_argument::emptyInput);
		ADD_TEST(TST_argument::nameOnlyDigitsRejected);
		ADD_TEST(TST_argument::nameStartingWithDigitRejected);
		ADD_TEST(TST_argument::missingPrefixRejected);
	}

private:
	using Argument = CommandLineParser::Argument;

	TEST_FUNCTION(flagShort)
	{
		TEST_START;
		Argument arg;
		TEST_ASSERT_TRUE(Argument::parse(arg, "-h"));
		TEST_COMPARE(arg.getName(), std::string("h"));
		TEST_COMPARE(arg.getValueCount(), (size_t)1);
		TEST_COMPARE(arg.getValue(), std::string(""));
	}

	TEST_FUNCTION(flagLong)
	{
		TEST_START;
		Argument arg;
		TEST_ASSERT_TRUE(Argument::parse(arg, "-help"));
		TEST_COMPARE(arg.getName(), std::string("help"));
	}

	TEST_FUNCTION(keyValue)
	{
		TEST_START;
		Argument arg;
		TEST_ASSERT_TRUE(Argument::parse(arg, "-port=8080"));
		TEST_COMPARE(arg.getName(), std::string("port"));
		TEST_COMPARE(arg.getValueCount(), (size_t)1);
		TEST_COMPARE(arg.getValue(), std::string("8080"));
	}

	// Regression: --no-gui used to be parsed as a flag named "no" with "-gui" silently dropped.
	TEST_FUNCTION(hyphenInName)
	{
		TEST_START;
		Argument arg;
		TEST_ASSERT_TRUE(Argument::parse(arg, "--no-gui"));
		TEST_COMPARE(arg.getName(), std::string("no-gui"));

		Argument arg2;
		TEST_ASSERT_TRUE(Argument::parse(arg2, "--dry-run=true"));
		TEST_COMPARE(arg2.getName(), std::string("dry-run"));
		TEST_COMPARE(arg2.getValue(), std::string("true"));
	}

	TEST_FUNCTION(underscoreInName)
	{
		TEST_START;
		Argument arg;
		TEST_ASSERT_TRUE(Argument::parse(arg, "-max_count=42"));
		TEST_COMPARE(arg.getName(), std::string("max_count"));
		TEST_COMPARE(arg.getValue(), std::string("42"));
	}

	TEST_FUNCTION(valueList)
	{
		TEST_START;
		Argument arg;
		TEST_ASSERT_TRUE(Argument::parse(arg, "-list=a;b;c"));
		TEST_COMPARE(arg.getName(), std::string("list"));
		TEST_COMPARE(arg.getValueCount(), (size_t)3);
		TEST_COMPARE(arg.getValue(0), std::string("a"));
		TEST_COMPARE(arg.getValue(1), std::string("b"));
		TEST_COMPARE(arg.getValue(2), std::string("c"));
	}

	TEST_FUNCTION(emptyInput)
	{
		TEST_START;
		Argument arg;
		TEST_ASSERT_FALSE(Argument::parse(arg, ""));
	}

	TEST_FUNCTION(nameOnlyDigitsRejected)
	{
		TEST_START;
		Argument arg;
		TEST_ASSERT_FALSE(Argument::parse(arg, "-123"));
	}

	TEST_FUNCTION(nameStartingWithDigitRejected)
	{
		TEST_START;
		Argument arg;
		TEST_ASSERT_FALSE(Argument::parse(arg, "-1abc"));
	}

	TEST_FUNCTION(missingPrefixRejected)
	{
		TEST_START;
		Argument arg;
		TEST_ASSERT_FALSE(Argument::parse(arg, "help"));
	}
};

TEST_INSTANTIATE(TST_argument);
