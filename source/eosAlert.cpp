#include "eosAlert.h"
#include "eosDesktop.h"

/*******************************************************************************
 * eos::Frame.
 ******************************************************************************/
eos::Alert::Alert(axWindow* parent,
                  const ax::Rect& rect,
                  const std::string& alert_message):
// Parent.
axPanel(parent, rect),
_alert_str(alert_message),
_font(0)
{
//    ax::Button::Info btn_info(ax::Color(0.5, 0.5, 0.5, 0.0),
//                              ax::Color(1.0, 0.0, 0.0),
//                              ax::Color(0.95, 0.0, 0.0),
//                              ax::Color(0.5, 0.5, 0.5, 0.0),
//                              ax::Color(0.0, 0.0, 0.0, 0.0),
//                              ax::Color(0.0, 0.0, 0.0),
//                              0);
//    
//    _close_btn = new ax::Button(this, ax::Rect(rect.size.x - 25 - 5 - 1, 0, 23, 25),
//                                ax::Button::Events(GetOnButtonClick()),
//                                btn_info, "", "x");
//    
//    btn_info.hover = ax::Color(0.0, 0.0, 1.0);
//    btn_info.clicking = ax::Color(0.0, 0.0, 0.95);
//    _min_btn = new ax::Button(this, ax::Rect(rect.size.x - 25 - 5 - 1 - 25, 0, 23, 25),
//                              ax::Button::Events(GetOnMinimize()),
//                              btn_info, "", "_");
}

void eos::Alert::OnPaint()
{
    ax::GC gc;
    ax::Rect rect(GetDrawingRect());
    
    gc.SetColor(ax::Color(0.95));
    gc.DrawRectangle(rect);
    
    gc.SetColor(ax::Color(0.0));
    gc.DrawString(_font, _alert_str, ax::Point(50, 50));
}


    
