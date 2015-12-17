#include "eosDesktop.h"
#include "eosDesktopIcon.h"
#include "eosCoreTracer.h"

#include "eosDesktopExplorer.h"

#include "axLib/axWindowTree.h"
#include "axLib/axWindowManager.h"
#include "axLib/axOSFileSystem.h"

eos::DesktopIcon::DesktopIcon(
	DesktopExplorer* explorer, const ax::Point& pos, const std::string& img_path, const std::string& name)
	: _explorer(explorer)
	, _clickPos(0, 0)
	, _current_color(0.8, 0.005)
	, _grid_index(0, 0)
	, _name(name)
	, _selected(false)
	, _font(0)
{
	_img = std::shared_ptr<ax::Image>(new ax::Image(img_path));

	win = ax::Window::Create(ax::Rect(pos, ax::Size(80, 80)));
	win->event.OnPaint = ax::WBind<ax::GC>(this, &DesktopIcon::OnPaint);
	win->event.OnMouseMotion = ax::WBind<ax::Point>(this, &DesktopIcon::OnMouseMotion);
	win->event.OnMouseLeftDown = ax::WBind<ax::Point>(this, &DesktopIcon::OnMouseLeftDown);
	win->event.OnMouseLeftUp = ax::WBind<ax::Point>(this, &DesktopIcon::OnMouseLeftUp);
	win->event.OnMouseLeftDragging = ax::WBind<ax::Point>(this, &DesktopIcon::OnMouseLeftDragging);
	win->event.OnMouseEnter = ax::WBind<ax::Point>(this, &DesktopIcon::OnMouseEnter);
	win->event.OnMouseLeave = ax::WBind<ax::Point>(this, &DesktopIcon::OnMouseLeave);
}

ax::Point eos::DesktopIcon::GetGridIndex() const
{
	return _grid_index;
}

void eos::DesktopIcon::SetGridIndex(const ax::Point& index)
{
	_grid_index = index;
}

void eos::DesktopIcon::OnMouseEnter(const ax::Point& mouse)
{
	_current_color = ax::Color(0.8, 0.1);
	_selected = true;
	win->Update();
}

void eos::DesktopIcon::OnMouseLeave(const ax::Point& mouse)
{
	_current_color = ax::Color(0.8, 0.005);
	_selected = false;
	win->Update();
}

void eos::DesktopIcon::OnMouseMotion(const ax::Point& mouse)
{
}

void eos::DesktopIcon::OnMouseLeftDown(const ax::Point& mouse)
{
	win->event.GrabMouse();

	_clickPos = ax::Point(mouse - win->dimension.GetAbsoluteRect().position);

	_explorer->HandlePickingIcon(win, _clickPos, mouse);
}

void eos::DesktopIcon::OnMouseLeftUp(const ax::Point& mouse)
{
	if (win->event.IsGrabbed()) {
		win->event.UnGrabMouse();
		_explorer->HandleDraggingReleaseIcon(win, _clickPos, mouse);
	}
}

void eos::DesktopIcon::OnMouseLeftDragging(const ax::Point& mouse)
{
	_explorer->HandleDraggingIcon(win, _clickPos, mouse);
}

void eos::DesktopIcon::OnPaint(ax::GC gc)
{
	ax::Rect rect(win->dimension.GetDrawingRect());

	if (_selected) {
		gc.SetColor(_current_color);
		gc.DrawRoundedRectangle(rect, 8);
	}

	if (_img) {
		gc.DrawImage(_img.get(), rect.position + ax::Point(8, 0));
	}

	// Draw icon name.
	gc.SetColor(ax::Color(0.9));
	gc.DrawStringAlignedCenter(_font, _name, ax::Rect(0, 62, rect.size.x, 16));
}