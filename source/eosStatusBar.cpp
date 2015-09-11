#include "eosStatusBar.h"
#include "eosDesktop.h"

#include "eosCoreUser.h"

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
    gc.DrawString(_font, _clock_str, ax::Point(75, 5));
}



eos::StatusBar::StatusBar(axWindow* parent,
                          const ax::Rect& rect,
                          eos::Core::System* system):
// Parent.
axPanel(parent, rect),
_font(0),
_system(system)
{
    _user_name = _system->GetUser()->GetFullName();
    
    Clock* clock = new Clock(this, ax::Rect(rect.size.x - 140, 0, 140, 25));
    
    
    
    ax::Button::Info btn_info(ax::Color(0.5, 0.5, 0.5, 0.0),
                              ax::Color(1.0, 0.0, 0.0, 0.0),
                              ax::Color(0.95, 0.0, 0.0, 0.0),
                              ax::Color(0.5, 0.5, 0.5, 0.0),
                              ax::Color(0.0, 0.0, 0.0, 0.0),
                              ax::Color(0.0, 0.0, 0.0, 0.0),
                              0);
    
    ax::Button* volume = new ax::Button(this, ax::Rect(rect.size.x - 310, 3, 20, 20),
                                      ax::Button::Events(),
                                      btn_info, "resource/volume51.png", "",
                                      ax::Button::Flags::SINGLE_IMG);
    
    
    
    
    ax::Button* view = new ax::Button(this, ax::Rect(6, 3, 20, 20),
                                      ax::Button::Events(GetOnHome()),
                                      btn_info, "resource/home.png", "",
                                      ax::Button::Flags::SINGLE_IMG);
    
    ax::Button* btn = new ax::Button(this, ax::Rect(view->GetRect().GetNextPosRight(10), ax::Size(20, 20)),
                                     ax::Button::Events(GetOnNotificationMode()),
                                     btn_info, "resource/sort52.png", "",
                                     ax::Button::Flags::SINGLE_IMG);
    
    ax::Button* btn2 = new ax::Button(this, ax::Rect(btn->GetRect().GetNextPosRight(10), ax::Size(20, 20)),
                                      ax::Button::Events(GetOnAppViewer()),
                                      btn_info, "resource/view.png", "",
                                      ax::Button::Flags::SINGLE_IMG);
    
    ax::Button* btn3 = new ax::Button(this, ax::Rect(btn2->GetRect().GetNextPosRight(10), ax::Size(20, 20)),
                                      ax::Button::Events(GetOnTerminalMode()),
                                      btn_info, "resource/terminal_mode.png", "",
                                      ax::Button::Flags::SINGLE_IMG);
    
    ax::Button* btn4 = new ax::Button(this, ax::Rect(btn3->GetRect().GetNextPosRight(10), ax::Size(20, 20)),
                                      ax::Button::Events(),
                                      btn_info, "resource/setting.png", "",
                                      ax::Button::Flags::SINGLE_IMG);
    
    ax::Button* btn5 = new ax::Button(this, ax::Rect(btn4->GetRect().GetNextPosRight(10), ax::Size(20, 20)),
                                      ax::Button::Events(GetOnTraceMode()),
                                      btn_info, "resource/list88.png", "",
                                      ax::Button::Flags::SINGLE_IMG);
    
    ax::Button* btn6 = new ax::Button(this, ax::Rect(btn5->GetRect().GetNextPosRight(10), ax::Size(20, 20)),
                                      ax::Button::Events(GetOnView()),
                                      btn_info, "resource/elipse.png", "",
                                      ax::Button::Flags::SINGLE_IMG);
    
    
}

void eos::StatusBar::OnView(const ax::Button::Msg& msg)
{
    eos::Desktop* desktop = static_cast<eos::Desktop*>(GetParent());
    desktop->ShowView();
}

void eos::StatusBar::OnNotificationMode(const ax::Button::Msg& msg)
{
    eos::Desktop* desktop = static_cast<eos::Desktop*>(GetParent());
    desktop->ToggleDesktopApp(eos::Desktop::DesktopApps::DSKT_APP_NOTIFY);
}

void eos::StatusBar::OnTerminalMode(const ax::Button::Msg& msg)
{
    eos::Desktop* desktop = static_cast<eos::Desktop*>(GetParent());
    desktop->ToggleDesktopApp(eos::Desktop::DesktopApps::DSKT_APP_TERMINAL);
}

void eos::StatusBar::OnTraceMode(const ax::Button::Msg& msg)
{
    eos::Desktop* desktop = static_cast<eos::Desktop*>(GetParent());
    desktop->ToggleDesktopApp(eos::Desktop::DesktopApps::DSKT_APP_TRACE);
}

void eos::StatusBar::OnHome(const ax::Button::Msg& msg)
{
    eos::Desktop* desktop = static_cast<eos::Desktop*>(GetParent());
    desktop->ToggleDesktopApp(eos::Desktop::DesktopApps::DSKT_APP_HOME);
}

void eos::StatusBar::OnAppViewer(const ax::Button::Msg& msg)
{
    eos::Desktop* desktop = static_cast<eos::Desktop*>(GetParent());
    desktop->ToggleDesktopApp(eos::Desktop::DesktopApps::DSKT_APP_VIEWER);
}

void eos::StatusBar::OnPaint()
{
    ax::GC gc;
    ax::Rect rect(GetDrawingRect());
    
    gc.SetColor(ax::Color(0.4, 0.4));
    gc.DrawRectangle(ax::Rect(0, 0, rect.size.x, rect.size.y));
    
    gc.SetColor(ax::Color(0.9));
    gc.DrawString(_font, _user_name, ax::Point(rect.size.x - 280, 5));
    
    gc.SetColor(ax::Color(0.4, 0.5));
    gc.DrawLine(ax::Point(0, rect.size.y), ax::Point(rect.size.x, rect.size.y));
}
