#include "eosCoreTracer.h"

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

eos::Core::Tracer::Tracer(ax::Event::Manager* evt_manager):
ax::Event::Object(evt_manager),
_f_name("resource/system_trace")
{
    remove(_f_name.c_str());
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