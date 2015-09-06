#include "eosImageViewer.h"

/*******************************************************************************
 * eos::ImageViewer.
 ******************************************************************************/
eos::ImageViewer::ImageViewer(axWindow* parent,
                              const ax::Rect& rect,
                              const std::string& img_path) :
// Parent.
axPanel(parent, rect)
{
    
}

void eos::ImageViewer::OnPaint()
{
    ax::GC gc;
    ax::Rect rect(GetDrawingRect());
    
    gc.SetColor(ax::Color(0.8, 0.8, 0.8));
    gc.DrawRectangle(rect);
}
