//
//  eosStatusBar.h
//  Minimal
//
//  Created by Alexandre Arsenault on 2015-08-25.
//  Copyright (c) 2015 axLib. All rights reserved.
//

#ifndef __Minimal__eosStatusBar__
#define __Minimal__eosStatusBar__

#include <ctime>
#include "axLib/axLib.h"
#include "axLib/axTimer.h"
#include "axLib/axButton.h"

/*******************************************************************************
 * Clock.
 ******************************************************************************/
class Clock : public axPanel
{
public:
    Clock(axWindow* parent, const ax::Rect& rect);
    
private:
    ax::Font _font;
    ax::Event::Timer* _timer_clock;
    std::string _clock_str, _date_str;
    
    axEVENT_ACCESSOR(ax::Event::Timer::Msg, OnTimerClock);
    void OnTimerClock(const ax::Event::Timer::Msg& msg);
    
    void OnPaint();
};

namespace eos
{
    /***************************************************************************
     * eos::StatusBar.
     **************************************************************************/
    class StatusBar : public axPanel
    {
    public:
        StatusBar(axWindow* parent, const ax::Rect& rect);
        
        
    private:
        
        
        ax::Image* _setting_img;
        ax::Font _font;
        ax::Event::Timer* _timer_clock;
        
        axEVENT_ACCESSOR(ax::Button::Msg, OnView);
        void OnView(const ax::Button::Msg& msg);
        
        axEVENT_ACCESSOR(ax::Button::Msg, OnNotificationMode);
        void OnNotificationMode(const ax::Button::Msg& msg);
        
        axEVENT_ACCESSOR(ax::Button::Msg, OnTerminalMode);
        void OnTerminalMode(const ax::Button::Msg& msg);
        
        axEVENT_ACCESSOR(ax::Button::Msg, OnTraceMode);
        void OnTraceMode(const ax::Button::Msg& msg);
        
        void OnPaint();
    };
}


#endif /* defined(__Minimal__eosStatusBar__) */
