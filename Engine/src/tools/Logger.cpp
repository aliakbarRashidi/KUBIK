#include "Logger.h"
#include "Utils.h"

using namespace kubik;
using namespace ci;
using namespace std;

bool Logger::init = false;
boost::filesystem::path Logger::logPath = "";

Logger& Logger::getInstance()
{
	static Logger logger;

	if (!init)
	{
		auto timeStruct = Utils::getCurrentTime();
		auto logName = to_string(1900 + timeStruct.tm_year) + "_"
			+ Utils::fix2(to_string(timeStruct.tm_mon + 1)) + "_"
			+ Utils::fix2(to_string(timeStruct.tm_mday)) + "-"
			+ Utils::fix2(to_string(timeStruct.tm_hour)) + "_"
			+ Utils::fix2(to_string(timeStruct.tm_min)) + "_"
			+ Utils::fix2(to_string(timeStruct.tm_sec)) + ".log";

		logPath = config::Paths::getLogPath(logName);		

		std::ofstream logfile;		
		logfile.open(logPath.c_str());
		logfile.close();

		init = true;
	}

	return logger;
}

void Logger::log(const std::string & message)
{
	std::ofstream logfile;
	logfile.open(logPath.c_str(), ios::out | ios::app);
	logfile << message << std::endl;
	logfile.close();

#ifdef CINDER_MSW
	OutputDebugStringA((message + '\n').c_str());
#endif
}

void Logger::log(const std::string & message, const std::string& filename)
{
	std::ofstream logfile;
	logfile.open(filename.c_str(), ios::out | ios::app);
	logfile << message << std::endl;
	logfile.close();
	
#ifdef CINDER_MSW
	OutputDebugStringA((message + '\n').c_str());
#endif
}
