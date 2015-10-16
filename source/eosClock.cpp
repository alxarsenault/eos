#include "eosClock.h"
#include "eosDesktop.h"

#include <ctime>

eos::Clock::Clock(const ax::Rect& rect)
: _font(0)
{
	win = ax::Window::Create(rect);
	
	win->event.OnPaint = ax::WBind<ax::GC>(this, &Clock::OnPaint);
	
	// Create timer.
	_timer_clock = std::shared_ptr<ax::Event::Timer>(
			new ax::Event::Timer(ax::App::GetInstance().GetEventManager(),
			ax::Event::Bind(this, &Clock::OnTimerClock)));
	
	// Update time every second (1000ms).
	_timer_clock->StartTimer(ax::Event::Timer::TimeMs(1000));
}

void eos::Clock::OnTimerClock(ax::Event::Msg* msg)
{
	time_t t = time(0);   // get time now
	struct tm * now = localtime( & t );
	
	int s = now->tm_sec;
	std::string sec = s < 10 ? "0" + std::to_string(s) : std::to_string(s);
	
	int m = now->tm_min;
	std::string min = m < 10 ? "0" + std::to_string(m) : std::to_string(m);
	
	std::string space(":");
	
	_clock_str = std::to_string(now->tm_hour) + space + min + space + sec;
	
	std::string slash("/");
	
	m = now->tm_mon + 1;
	std::string month = m < 10 ? "0" + std::to_string(m) : std::to_string(m);
	
	int d = now->tm_mday;
	std::string day = d < 10 ? "0" + std::to_string(d) : std::to_string(d);
	
	_date_str = day + slash + month + slash + std::to_string(now->tm_year + 1900);
	
	win->Update();
}

void eos::Clock::OnPaint(ax::GC gc)
{
	gc.SetColor(ax::Color(1.0));
	gc.DrawString(_font, _date_str, ax::Point(0, 5));
	gc.DrawString(_font, _clock_str, ax::Point(75, 5));
}
