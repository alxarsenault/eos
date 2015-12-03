#ifndef __EOS_CLOCK_H__
#define __EOS_CLOCK_H__

#include "axLib/axLib.h"
#include "axLib/axTimer.h"

namespace eos {
	/*
	 * eos::Clock.
	 */
	class Clock : public ax::Window::Backbone
	{
	public:
		Clock(const ax::Rect& rect);
		
	private:
		ax::Font _font;
		std::shared_ptr<ax::Event::Timer> _timer_clock;
		std::string _clock_str, _date_str;
		
		void OnTimerClock(ax::Event::Msg* msg);
		
		void OnPaint(ax::GC gc);
	};
}

#endif // __EOS_CLOCK_H__
