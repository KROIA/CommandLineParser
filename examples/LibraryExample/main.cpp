#ifdef QT_ENABLED
#include <QApplication>
#endif
#include <iostream>
#include "CommandLineParser.h"

#ifdef QT_WIDGETS_ENABLED
#include <QWidget>
#endif


int main(int argc, char* argv[])
{
#ifdef QT_WIDGETS_ENABLED
	QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
	QGuiApplication::setAttribute(Qt::AA_UseHighDpiPixmaps);
	QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
#endif
#ifdef QT_ENABLED
	QApplication app(argc, argv);
#endif
	CommandLineParser::Profiler::start();
	CommandLineParser::LibraryInfo::printInfo();

	for (int i = 0; i < argc; i++)
	{
		std::cout << "Argument " << i << ": " << argv[i] << std::endl;
	}

	std::string arguments = " -a=Hello -UA";
	char* argv2[] = { argv[0], (char*)arguments.c_str(),"AAA","-v","-k=5"};
	int argc2 = 5;
	//::string arguments = "-help -v=50 -a=\"Hello\" -v5 -test=\"\\\"\\nHel\n\\\"lo\\\"\" -list=\"a;b;c\" -list2=1;2;3;\"ffes fesf\";5;\"Hello \\\" \\; jkli\";";

	CommandLineParser::ArgumentList list;
	list.parse(argc, argv);
	//CommandLineParser::Argument::parse(list, argc2, argv2);
	for (size_t i = 0; i < list.size(); i++)
	{
		if (list[i].getValueCount() > 1)
		{
			std::cout << "Parsed Argument " << i << ": \"" << list[i].getName() << "\" = \"";
			for (size_t j = 0; j < list[i].getValueCount(); j++)
			{
				std::cout << list[i].getValue(j);
				if (j < list[i].getValueCount() - 1)
					std::cout << "\", \"";
			}
			std::cout <<"\""<< std::endl;
		}
		else if(list[i].getValueCount() == 1)
			std::cout << "Parsed Argument " << i << ": \"" << list[i].getName() << "\" = \"" << list[i].getValue() << "\"" << std::endl;
		else if(list[i].getValueCount() == 0)
			std::cout << "Parsed Argument " << i << ": \"" << list[i].getName() << "\"" << std::endl;
	}
	

#ifdef QT_WIDGETS_ENABLED
	QWidget* widget = CommandLineParser::LibraryInfo::createInfoWidget();
	if (widget)
		widget->show();
#endif
	int ret = 0;
#ifdef QT_ENABLED
	ret = app.exec();
#endif
	CommandLineParser::Profiler::stop((std::string(CommandLineParser::LibraryInfo::name)+".prof").c_str());
	return ret;
}