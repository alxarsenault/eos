//
//  eosStatusBar.cpp
//  Minimal
//
//  Created by Alexandre Arsenault on 2015-08-25.
//  Copyright (c) 2015 axLib. All rights reserved.
//

#include "eosStatusBar.h"

Clock::Clock(axWindow* parent, const ax::Rect& rect):
// Parent.
axPanel(parent, rect),
_font(0)
{
    _timer_clock = new ax::Event::Timer(GetApp()->GetEventManager(),
                                        GetOnTimerClock());
    
    _timer_clock->StartTimer(ax::Event::Timer::TimeMs(1000));
    
}

void Clock::OnTimerClock(const ax::Event::Timer::Msg& msg)
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
    
    Update();
}

void Clock::OnPaint()
{
    ax::GC gc;
    ax::Rect rect(GetDrawingRect());

    gc.SetColor(ax::Color(1.0));
    gc.DrawString(_font, _date_str, ax::Point(0, 5));
    gc.DrawString(_font, _clock_str, ax::Point(80, 5));
}

eos::StatusBar::StatusBar(axWindow* parent, const ax::Rect& rect):
// Parent.
axPanel(parent, rect),
_font(0)
{
    Clock* clock = new Clock(this, ax::Rect(rect.size.x - 140, 0, 140, 25));
}

void eos::StatusBar::OnPaint()
{
    ax::GC gc;
    ax::Rect rect(GetDrawingRect());
    
    gc.SetColor(ax::Color(0.4, 0.4));
    gc.DrawRectangle(rect);
}
