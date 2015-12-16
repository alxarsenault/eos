#include "eosDockIcon.h"

#include "axLib/axWindowManager.h"
#include "eosSystemProxy.h"

eos::DockIcon::DockIcon(const ax::Rect& rect, const std::string& img_path, const std::string& name)
	: _name(name)
{
	win = ax::Window::Create(rect);
	win->event.OnPaint = ax::WBind<ax::GC>(this, &DockIcon::OnPaint);
	win->event.OnMouseEnter = ax::WBind<ax::Point>(this, &DockIcon::OnMouseEnter);
	win->event.OnMouseLeave = ax::WBind<ax::Point>(this, &DockIcon::OnMouseLeave);
	win->event.OnMouseLeftDown = ax::WBind<ax::Point>(this, &DockIcon::OnMouseLeftDown);
	_img = std::shared_ptr<ax::Image>(new ax::Image(img_path));

	ax::Button::Info btn_info(ax::Color(0.5f, 0.5f, 0.5f, 0.0f), ax::Color(1.0f, 0.0f, 0.0f, 0.0f),
		ax::Color(0.95, 0.0, 0.0, 0.0f), ax::Color(0.5f, 0.5f, 0.5f, 0.0f), ax::Color(0.0f, 0.0f, 0.0f, 0.0f),
		ax::Color(0.0f, 0.0f, 0.0f, 0.0f), 0);
	
	ax::Button::Events btn_evts;
	btn_evts.button_click = ax::Event::Function([&](ax::Event::Msg* msg) {
		eos::sys::proxy::UnFullScreenFrame(_name);
		_close_full_screen_btn->GetWindow()->Hide();
	});
	
	_close_full_screen_btn = std::shared_ptr<ax::Button>(new ax::Button(
		ax::Rect(0, 0, 20, 20), btn_evts, btn_info, "resources/close_app.png"));
	
	win->node.Add(_close_full_screen_btn)->Hide();
}

void eos::DockIcon::ActivateFullScreenBtn()
{
	_close_full_screen_btn->GetWindow()->Show();
}

void eos::DockIcon::OnMouseLeftDown(const ax::Point& mouse)
{
	// Dock.
	win->node.GetParent()->PushEvent(320, new ax::Event::StringMsg(_name));
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
