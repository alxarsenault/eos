#include "eosNotification.h"
#include "eosDesktop.h"

eos::Notification::Notification(const ax::Rect& rect)
{
	win = ax::Window::Create(rect);
	
	win->event.OnPaint = ax::WBind<ax::GC>(this, &Notification::OnPaint);
}

void eos::Notification::OnPaint(ax::GC gc)
{
    ax::Rect rect(win->dimension.GetDrawingRect());
    
    gc.SetColor(ax::Color(0.4, 0.4));
    gc.DrawRectangle(ax::Rect(0, 0, rect.size.x, rect.size.y));
    
    gc.SetColor(ax::Color(0.4, 0.5));
    gc.DrawRectangleContour(ax::Rect(1, 0, rect.size.x, rect.size.y));
}
