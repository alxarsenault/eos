#include "eosDockIcon.h"

eos::DockIcon::DockIcon(const ax::Rect& rect, const std::string& img_path,
		const std::string& name)
	: _name(name)
{
	win = ax::Window::Create(rect);
	win->event.OnPaint = ax::WBind<ax::GC>(this, &DockIcon::OnPaint);
	
	_img = std::shared_ptr<ax::Image>(new ax::Image(img_path));
}

void eos::DockIcon::OnMouseLeave(const ax::Point& mouse)
{
//	ax::Core::WindowManager* wm = ax::App::GetMainApp()->GetWindowManager();
//	bool over_parent = wm->IsMouseHoverWindow(GetParent());
//	
//	if(!over_parent)
//	{
//		GetParent()->OnMouseLeave();
//	}
}

void eos::DockIcon::OnPaint(ax::GC gc)
{
	ax::Rect rect(win->dimension.GetDrawingRect());

	if (_img && _img->IsImageReady()) {
		gc.DrawImageResize(_img.get(), ax::Point(0, 0), rect.size);
	}
}



