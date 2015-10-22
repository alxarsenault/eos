#include "eosDockIcon.h"

#include "axLib/axWindowManager.h"

eos::DockIcon::DockIcon(const ax::Rect& rect, const std::string& img_path,
		const std::string& name)
	: _name(name)
{
	win = ax::Window::Create(rect);
	win->event.OnPaint = ax::WBind<ax::GC>(this, &DockIcon::OnPaint);
	win->event.OnMouseEnter = ax::WBind<ax::Point>(this, &DockIcon::OnMouseEnter);
	win->event.OnMouseLeave = ax::WBind<ax::Point>(this, &DockIcon::OnMouseLeave);
	
	_img = std::shared_ptr<ax::Image>(new ax::Image(img_path));
}


void eos::DockIcon::OnMouseLeave(const ax::Point& mouse)
{
	win->node.GetParent()->event.OnMouseLeave(mouse);
	
//	ax::App::GetInstance().GetWindowManager()->
//			SetPastWindow(win->node.GetParent());
}

void eos::DockIcon::OnMouseEnter(const ax::Point& mouse)
{
//	ax::App::GetInstance().GetWindowManager()->
//		SetPastWindow(win->node.GetParent());
}

void eos::DockIcon::OnPaint(ax::GC gc)
{
	ax::Rect rect(win->dimension.GetDrawingRect());

	if (_img && _img->IsImageReady()) {
		gc.DrawImageResize(_img.get(), ax::Point(0, 0), rect.size);
	}
}



