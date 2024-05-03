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

	std::string arguments = "-help -v=50 -a=\"Hello\" -v5 -test=\"\\\"\\nHel\n\\\"lo\\\"\" -list=\"a;b;c\" -list2=1;2;3;\"ffes fesf\";5;\"Hello \\\" \\; jkli\";";

	std::vector<CommandLineParser::Argument> list;
	CommandLineParser::Argument::parse(list,arguments);
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