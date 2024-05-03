#pragma once
#include "CommandLineParser_global.h"

/// USER_SECTION_START 1

/// USER_SECTION_END

// Debugging
#ifdef NDEBUG
	#define CLP_CONSOLE(msg)
	#define CLP_CONSOLE_FUNCTION(msg)
#else
	#include <iostream>

	#define CLP_DEBUG
	#define CLP_CONSOLE_STREAM std::cout

	#define CLP_CONSOLE(msg) CLP_CONSOLE_STREAM << msg;
	#define CLP_CONSOLE_FUNCTION(msg) CLP_CONSOLE_STREAM << __PRETTY_FUNCTION__ << " " << msg;
#endif

/// USER_SECTION_START 2

/// USER_SECTION_END

#ifdef CLP_PROFILING
	#include "easy/profiler.h"
	#include <easy/arbitrary_value.h> // EASY_VALUE, EASY_ARRAY are defined here

	#define CLP_PROFILING_BLOCK_C(text, color) EASY_BLOCK(text, color)
	#define CLP_PROFILING_NONSCOPED_BLOCK_C(text, color) EASY_NONSCOPED_BLOCK(text, color)
	#define CLP_PROFILING_END_BLOCK EASY_END_BLOCK
	#define CLP_PROFILING_FUNCTION_C(color) EASY_FUNCTION(color)
	#define CLP_PROFILING_BLOCK(text, colorStage) CLP_PROFILING_BLOCK_C(text,profiler::colors::  colorStage)
	#define CLP_PROFILING_NONSCOPED_BLOCK(text, colorStage) CLP_PROFILING_NONSCOPED_BLOCK_C(text,profiler::colors::  colorStage)
	#define CLP_PROFILING_FUNCTION(colorStage) CLP_PROFILING_FUNCTION_C(profiler::colors:: colorStage)
	#define CLP_PROFILING_THREAD(name) EASY_THREAD(name)

	#define CLP_PROFILING_VALUE(name, value) EASY_VALUE(name, value)
	#define CLP_PROFILING_TEXT(name, value) EASY_TEXT(name, value)

#else
	#define CLP_PROFILING_BLOCK_C(text, color)
	#define CLP_PROFILING_NONSCOPED_BLOCK_C(text, color)
	#define CLP_PROFILING_END_BLOCK
	#define CLP_PROFILING_FUNCTION_C(color)
	#define CLP_PROFILING_BLOCK(text, colorStage)
	#define CLP_PROFILING_NONSCOPED_BLOCK(text, colorStage)
	#define CLP_PROFILING_FUNCTION(colorStage)
	#define CLP_PROFILING_THREAD(name)

	#define CLP_PROFILING_VALUE(name, value)
	#define CLP_PROFILING_TEXT(name, value)
#endif

// Special expantion tecniques are required to combine the color name
#define CONCAT_SYMBOLS_IMPL(x, y) x##y
#define CONCAT_SYMBOLS(x, y) CONCAT_SYMBOLS_IMPL(x, y)



// Different color stages
#define CLP_COLOR_STAGE_1 50
#define CLP_COLOR_STAGE_2 100
#define CLP_COLOR_STAGE_3 200
#define CLP_COLOR_STAGE_4 300
#define CLP_COLOR_STAGE_5 400
#define CLP_COLOR_STAGE_6 500
#define CLP_COLOR_STAGE_7 600
#define CLP_COLOR_STAGE_8 700
#define CLP_COLOR_STAGE_9 800
#define CLP_COLOR_STAGE_10 900
#define CLP_COLOR_STAGE_11 A100 
#define CLP_COLOR_STAGE_12 A200 
#define CLP_COLOR_STAGE_13 A400 
#define CLP_COLOR_STAGE_14 A700 

namespace CommandLineParser
{
	class COMMAND_LINE_PARSER_EXPORT Profiler
	{
	public:
		// Implementation defined in LibraryName_info.cpp to save files.
		static void start();
		static void stop();
		static void stop(const char* profilerOutputFile);
	};
}


// General
#define CLP_GENERAL_PROFILING_COLORBASE Cyan
#define CLP_GENERAL_PROFILING_BLOCK_C(text, color) CLP_PROFILING_BLOCK_C(text, color)
#define CLP_GENERAL_PROFILING_NONSCOPED_BLOCK_C(text, color) CLP_PROFILING_NONSCOPED_BLOCK_C(text, color)
#define CLP_GENERAL_PROFILING_END_BLOCK CLP_PROFILING_END_BLOCK;
#define CLP_GENERAL_PROFILING_FUNCTION_C(color) CLP_PROFILING_FUNCTION_C(color)
#define CLP_GENERAL_PROFILING_BLOCK(text, colorStage) CLP_PROFILING_BLOCK(text, CONCAT_SYMBOLS(CLP_GENERAL_PROFILING_COLORBASE, colorStage))
#define CLP_GENERAL_PROFILING_NONSCOPED_BLOCK(text, colorStage) CLP_PROFILING_NONSCOPED_BLOCK(text, CONCAT_SYMBOLS(CLP_GENERAL_PROFILING_COLORBASE, colorStage))
#define CLP_GENERAL_PROFILING_FUNCTION(colorStage) CLP_PROFILING_FUNCTION(CONCAT_SYMBOLS(CLP_GENERAL_PROFILING_COLORBASE, colorStage))
#define CLP_GENERAL_PROFILING_VALUE(name, value) CLP_PROFILING_VALUE(name, value)
#define CLP_GENERAL_PROFILING_TEXT(name, value) CLP_PROFILING_TEXT(name, value)


/// USER_SECTION_START 3

/// USER_SECTION_END