#include "eosCoreTracer.h"

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

eos::Core::Tracer::Tracer()
	: ax::Event::Object(ax::App::GetInstance().GetEventManager())
	, _f_name("resources/system_trace")
{
	remove(_f_name.c_str()); // Delete past tracer file.
	_fstream.open(_f_name.c_str(), std::fstream::out | std::fstream::app);
}

eos::Core::Tracer::~Tracer()
{
	_fstream.close();
}

const std::string eos::Core::Tracer::GetTraceFilePath() const
{
	return _f_name;
}

void eos::Core::Tracer::Print()
{
	_fstream << std::endl;
	PushEvent(0, new ax::Event::SimpleMsg<int>(0));
}

std::string eos::Core::Tracer::GetTimeStamp() const
{
	timeval tv;
	time_t t = time(0);
	gettimeofday(&tv, NULL);
	tm* now = localtime(&t);
	
	int hours = now->tm_hour;
	int minutes = (int)((tv.tv_sec % 3600) / 60);
	int seconds = (int)(tv.tv_sec % 60);
	int ms = (int)(tv.tv_usec / 1000);
	
	std::ostringstream oss;
	oss << std::right << std::setw(2) << std::setfill('0') << hours << ':'
	<< std::right << std::setw(2) << std::setfill('0') << minutes << ':'
	<< std::right << std::setw(2) << std::setfill('0') << seconds << ':'
	<< std::right << std::setw(3) << std::setfill('0') << ms;
	
	return oss.str();
}