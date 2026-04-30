#pragma once

#include "UnitTest.h"
#include "CommandLineParser.h"
#include <vector>
#include <string>

class TST_argumentList : public UnitTest::Test
{
	TEST_CLASS(TST_argumentList)
public:
	TST_argumentList()
		: Test("TST_argumentList")
	{
		ADD_TEST(TST_argumentList::argv_singleFlag);
		ADD_TEST(TST_argumentList::argv_keyValue);
		ADD_TEST(TST_argumentList::argv_hyphenName);
		ADD_TEST(TST_argumentList::argv_multipleFlags);
		ADD_TEST(TST_argumentList::argv_unknownTokensIgnored);
		ADD_TEST(TST_argumentList::argv_spaceSeparatedValue);
		ADD_TEST(TST_argumentList::argv_spaceSeparatedListValue);
		ADD_TEST(TST_argumentList::argv_flagBeforeFlagHasNoValue);
		ADD_TEST(TST_argumentList::argv_mixedSpaceAndEqualsForms);
		ADD_TEST(TST_argumentList::str_singleFlag);
		ADD_TEST(TST_argumentList::str_keyValueScalar);
		ADD_TEST(TST_argumentList::str_keyValueList);
		ADD_TEST(TST_argumentList::str_quotedValueWithSpaces);
		ADD_TEST(TST_argumentList::str_quotedValueWithSeparator);
		ADD_TEST(TST_argumentList::str_hyphenName);
		ADD_TEST(TST_argumentList::str_multipleArgs);
		ADD_TEST(TST_argumentList::hasArgument_lookup);
		ADD_TEST(TST_argumentList::getValueIfExists_lookup);
	}

private:
	using Argument = CommandLineParser::Argument;
	using ArgumentList = CommandLineParser::ArgumentList;

	// Build a fake argv. The first slot is a fake program name, like a real argv[0].
	static std::vector<char*> makeArgv(std::vector<std::string>& storage,
	                                   const std::vector<std::string>& args)
	{
		storage.clear();
		storage.reserve(args.size() + 1);
		storage.push_back("prog");
		for (const auto& a : args)
			storage.push_back(a);
		std::vector<char*> argv;
		argv.reserve(storage.size());
		for (auto& s : storage)
			argv.push_back(const_cast<char*>(s.c_str()));
		return argv;
	}

	TEST_FUNCTION(argv_singleFlag)
	{
		TEST_START;
		std::vector<std::string> storage;
		auto argv = makeArgv(storage, { "-h" });
		ArgumentList list;
		TEST_ASSERT_TRUE(list.parse((int)argv.size(), argv.data()));
		TEST_COMPARE(list.size(), (size_t)1);
		TEST_ASSERT_TRUE(list.hasArgument("h"));
	}

	TEST_FUNCTION(argv_keyValue)
	{
		TEST_START;
		std::vector<std::string> storage;
		auto argv = makeArgv(storage, { "-port=8080" });
		ArgumentList list;
		TEST_ASSERT_TRUE(list.parse((int)argv.size(), argv.data()));
		TEST_COMPARE(list.size(), (size_t)1);
		TEST_COMPARE(list.getArgument("port").getValue(), std::string("8080"));
	}

	// Regression: --no-gui must round-trip as a single argument named "no-gui".
	TEST_FUNCTION(argv_hyphenName)
	{
		TEST_START;
		std::vector<std::string> storage;
		auto argv = makeArgv(storage, { "--no-gui", "--dry-run=true" });
		ArgumentList list;
		TEST_ASSERT_TRUE(list.parse((int)argv.size(), argv.data()));
		TEST_COMPARE(list.size(), (size_t)2);
		TEST_ASSERT_TRUE(list.hasArgument("no-gui"));
		TEST_ASSERT_TRUE(list.hasArgument("dry-run"));
		TEST_ASSERT_FALSE(list.hasArgument("no"));
		TEST_ASSERT_FALSE(list.hasArgument("gui"));
		TEST_COMPARE(list.getArgument("dry-run").getValue(), std::string("true"));
	}

	TEST_FUNCTION(argv_multipleFlags)
	{
		TEST_START;
		std::vector<std::string> storage;
		auto argv = makeArgv(storage, { "-a", "-b", "-c=1" });
		ArgumentList list;
		TEST_ASSERT_TRUE(list.parse((int)argv.size(), argv.data()));
		TEST_COMPARE(list.size(), (size_t)3);
		TEST_ASSERT_TRUE(list.hasArgument("a"));
		TEST_ASSERT_TRUE(list.hasArgument("b"));
		TEST_COMPARE(list.getArgument("c").getValue(), std::string("1"));
	}

	TEST_FUNCTION(argv_unknownTokensIgnored)
	{
		TEST_START;
		std::vector<std::string> storage;
		// A bare token immediately after a flag is consumed as that flag's value
		// (POSIX-style). It must NOT be parsed as a separate argument.
		auto argv = makeArgv(storage, { "-v", "input.txt" });
		ArgumentList list;
		list.parse((int)argv.size(), argv.data());
		TEST_ASSERT_TRUE(list.hasArgument("v"));
		TEST_ASSERT_FALSE(list.hasArgument("input.txt"));
		TEST_COMPARE(list.getArgument("v").getValue(), std::string("input.txt"));
	}

	// Regression: --specFile /path was previously parsed as a flag with an empty
	// value (the path was silently dropped). It must round-trip as a value.
	TEST_FUNCTION(argv_spaceSeparatedValue)
	{
		TEST_START;
		std::vector<std::string> storage;
		auto argv = makeArgv(storage, { "--specFile", "/tmp/x.spec" });
		ArgumentList list;
		TEST_ASSERT_TRUE(list.parse((int)argv.size(), argv.data()));
		TEST_COMPARE(list.size(), (size_t)1);
		std::string v;
		TEST_ASSERT_TRUE(list.getValueIfExists("specFile", v));
		TEST_COMPARE(v, std::string("/tmp/x.spec"));
	}

	TEST_FUNCTION(argv_spaceSeparatedListValue)
	{
		TEST_START;
		std::vector<std::string> storage;
		auto argv = makeArgv(storage, { "--items", "a;b;c" });
		ArgumentList list;
		TEST_ASSERT_TRUE(list.parse((int)argv.size(), argv.data()));
		Argument a = list.getArgument("items");
		TEST_COMPARE(a.getValueCount(), (size_t)3);
		TEST_COMPARE(a.getValue(0), std::string("a"));
		TEST_COMPARE(a.getValue(2), std::string("c"));
	}

	// "--a --b" must parse as two flags, NOT as flag "a" with value "--b".
	TEST_FUNCTION(argv_flagBeforeFlagHasNoValue)
	{
		TEST_START;
		std::vector<std::string> storage;
		auto argv = makeArgv(storage, { "--a", "--b" });
		ArgumentList list;
		TEST_ASSERT_TRUE(list.parse((int)argv.size(), argv.data()));
		TEST_COMPARE(list.size(), (size_t)2);
		TEST_ASSERT_TRUE(list.hasArgument("a"));
		TEST_ASSERT_TRUE(list.hasArgument("b"));
		TEST_COMPARE(list.getArgument("a").getValue(), std::string(""));
	}

	TEST_FUNCTION(argv_mixedSpaceAndEqualsForms)
	{
		TEST_START;
		std::vector<std::string> storage;
		auto argv = makeArgv(storage, { "--headless", "--specFile=/tmp/a", "--outputPath", "/tmp/b", "--no-gui" });
		ArgumentList list;
		TEST_ASSERT_TRUE(list.parse((int)argv.size(), argv.data()));
		TEST_COMPARE(list.size(), (size_t)4);
		TEST_ASSERT_TRUE(list.hasArgument("headless"));
		TEST_ASSERT_TRUE(list.hasArgument("no-gui"));
		TEST_COMPARE(list.getArgument("specFile").getValue(), std::string("/tmp/a"));
		TEST_COMPARE(list.getArgument("outputPath").getValue(), std::string("/tmp/b"));
	}

	TEST_FUNCTION(str_singleFlag)
	{
		TEST_START;
		ArgumentList list;
		TEST_ASSERT_TRUE(list.parse(std::string("-h")));
		TEST_COMPARE(list.size(), (size_t)1);
		TEST_ASSERT_TRUE(list.hasArgument("h"));
	}

	// Regression: parse(string) used to lose scalar values because the splitter
	// only pushed when it saw a `;`.
	TEST_FUNCTION(str_keyValueScalar)
	{
		TEST_START;
		ArgumentList list;
		TEST_ASSERT_TRUE(list.parse(std::string("-port=8080")));
		TEST_COMPARE(list.size(), (size_t)1);
		Argument a = list.getArgument("port");
		TEST_COMPARE(a.getValueCount(), (size_t)1);
		TEST_COMPARE(a.getValue(), std::string("8080"));
	}

	TEST_FUNCTION(str_keyValueList)
	{
		TEST_START;
		ArgumentList list;
		TEST_ASSERT_TRUE(list.parse(std::string("-list=a;b;c")));
		Argument a = list.getArgument("list");
		TEST_COMPARE(a.getValueCount(), (size_t)3);
		TEST_COMPARE(a.getValue(0), std::string("a"));
		TEST_COMPARE(a.getValue(1), std::string("b"));
		TEST_COMPARE(a.getValue(2), std::string("c"));
	}

	TEST_FUNCTION(str_quotedValueWithSpaces)
	{
		TEST_START;
		ArgumentList list;
		TEST_ASSERT_TRUE(list.parse(std::string("-msg=\"hello world\"")));
		Argument a = list.getArgument("msg");
		TEST_COMPARE(a.getValueCount(), (size_t)1);
		TEST_COMPARE(a.getValue(), std::string("hello world"));
	}

	// A `;` inside quotes must NOT split the value.
	TEST_FUNCTION(str_quotedValueWithSeparator)
	{
		TEST_START;
		ArgumentList list;
		TEST_ASSERT_TRUE(list.parse(std::string("-list=\"a;b\";c")));
		Argument a = list.getArgument("list");
		TEST_COMPARE(a.getValueCount(), (size_t)2);
		TEST_COMPARE(a.getValue(0), std::string("a;b"));
		TEST_COMPARE(a.getValue(1), std::string("c"));
	}

	TEST_FUNCTION(str_hyphenName)
	{
		TEST_START;
		ArgumentList list;
		TEST_ASSERT_TRUE(list.parse(std::string("--no-gui --dry-run=true")));
		TEST_COMPARE(list.size(), (size_t)2);
		TEST_ASSERT_TRUE(list.hasArgument("no-gui"));
		TEST_COMPARE(list.getArgument("dry-run").getValue(), std::string("true"));
	}

	TEST_FUNCTION(str_multipleArgs)
	{
		TEST_START;
		ArgumentList list;
		TEST_ASSERT_TRUE(list.parse(std::string("-a -b=2 -c=x;y")));
		TEST_COMPARE(list.size(), (size_t)3);
		TEST_COMPARE(list.getArgument("b").getValue(), std::string("2"));
		TEST_COMPARE(list.getArgument("c").getValueCount(), (size_t)2);
	}

	TEST_FUNCTION(hasArgument_lookup)
	{
		TEST_START;
		ArgumentList list;
		list.parse(std::string("-a -b=1"));
		TEST_ASSERT_TRUE(list.hasArgument("a"));
		TEST_ASSERT_TRUE(list.hasArgument("b"));
		TEST_ASSERT_FALSE(list.hasArgument("c"));
		std::vector<std::string> any = { "x", "b" };
		TEST_ASSERT_TRUE(list.hasArgument(any));
	}

	TEST_FUNCTION(getValueIfExists_lookup)
	{
		TEST_START;
		ArgumentList list;
		list.parse(std::string("-port=9090"));
		std::string v;
		TEST_ASSERT_TRUE(list.getValueIfExists("port", v));
		TEST_COMPARE(v, std::string("9090"));
		TEST_ASSERT_FALSE(list.getValueIfExists("missing", v));
	}
};

TEST_INSTANTIATE(TST_argumentList);
