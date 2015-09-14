#include "eosHome.h"
#include "eosDesktop.h"

eos::Home::Home(ax::Window* parent, const ax::Rect& rect):
// Parent.
axPanel(parent, rect)
{
    ax::Button::Info btn_info(ax::Color(0.5, 0.5, 0.5, 0.0),
                              ax::Color(1.0, 0.0, 0.0, 0.0),
                              ax::Color(0.95, 0.0, 0.0, 0.0),
                              ax::Color(0.5, 0.5, 0.5, 0.0),
                              ax::Color(0.0, 0.0, 0.0, 0.0),
                              ax::Color(0.0, 0.0, 0.0, 0.0),
                              0);

    std::vector<std::string> btns =
    {
        std::string("resource/screen44.png"),
        std::string("resource/shared1.png"),
        std::string("resource/work3.png"),
        std::string("resource/tack.png"),
        std::string("resource/mountain31.png"),
        std::string("resource/film61.png"),
        std::string("resource/graduate32.png"),
        std::string("resource/black402.png"),
        std::string("resource/favorite21.png"),
        std::string("resource/facebook56.png"),
        std::string("resource/dark55.png"),
        std::string("resource/clipboard99.png"),
        std::string("resource/attachment19.png")
    };
    
    ax::Size btnSize(20, 20);
    ax::Point btnPos(6, 5);
    
    for(auto& n : btns)
    {
        ax::Button* btn = new ax::Button(this, ax::Rect(btnPos, btnSize),
                                         ax::Button::Events(),
                                         btn_info, n, "",
                                         ax::Button::Flags::SINGLE_IMG);
        
        btnPos = btn->GetRect().GetNextPosDown(5);
    } 
}

void eos::Home::OnPaint()
{
    ax::GC gc;
    ax::Rect rect(GetDrawingRect());
    
    gc.SetColor(ax::Color(0.4, 0.4));
    gc.DrawRectangle(ax::Rect(0, 0, rect.size.x, rect.size.y));
    
    gc.SetColor(ax::Color(0.4, 0.5));
    gc.DrawRectangleContour(ax::Rect(1, 0, rect.size.x, rect.size.y));
}
