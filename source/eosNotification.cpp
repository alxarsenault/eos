#include "eosNotification.h"
#include "eosDesktop.h"

eos::Notification::Notification(axWindow* parent, const ax::Rect& rect):
// Parent.
axPanel(parent, rect)
{
}

void eos::Notification::OnPaint()
{
    ax::GC gc;
    ax::Rect rect(GetDrawingRect());
    
    gc.SetColor(ax::Color(0.4, 0.4));
    gc.DrawRectangle(ax::Rect(0, 0, rect.size.x, rect.size.y));
}
