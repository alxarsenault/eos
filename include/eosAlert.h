#ifndef __EOS_ALERT_H__
#define __EOS_ALERT_H__

#include "axLib/axLib.h"
#include <fstream>

namespace eos {
namespace Core {
	class System;
}

class Alert : public ax::Window::Backbone {
public:
	Alert(const ax::Rect& rect);

private:
	ax::Font _font, _time_font;
	eos::Core::System* _system;
	std::fstream _tracer_stream;
	ax::StringVector _trace;
	ax::StringVector _trace_time;
	std::size_t _read_pos;

	axEVENT_ACCESSOR(ax::Event::SimpleMsg<int>, OnTracer);
	void OnTracer(const ax::Event::SimpleMsg<int>& msg);

	void OnPaint(ax::GC gc);
};
}

#endif //__EOS_ALERT_H__
