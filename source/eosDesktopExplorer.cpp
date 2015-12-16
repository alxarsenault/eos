#include "eosDesktopExplorer.h"
#include "eosDesktopIcon.h"
#include "eosDesktop.h"

#include "axLib/axOSFileSystem.h"

namespace eos {

const int DesktopExplorer::_delta_icon = 84;

DesktopExplorer::DesktopExplorer(eos::Desktop* desktop)
	: _desktop(desktop)
	, _has_icon_dragging(false)
	, _grid_index(0, 0)
	, _over_lapping_grid_index(-1, -1)
	, _selection(false, ax::Rect(0, 0, 0, 0))
{
}

void DesktopExplorer::HandlePickingIcon(
	ax::Window::Ptr icon, const ax::Point& click_pos, const ax::Point& mouse)
{
	ax::Point icon_pos(BlockIconWithDesktopBorder(icon, click_pos, mouse));

	std::shared_ptr<DesktopIcon> dIcon(std::static_pointer_cast<DesktopIcon>(icon->backbone));

	_grid_index = FindIconGridIndex(dIcon->GetGridIndex(), icon_pos);

	_desktop->GetWindow()->Update();
}

void DesktopExplorer::HandleDraggingIcon(
	ax::Window::Ptr icon, const ax::Point& click_pos, const ax::Point& mouse)
{
	_has_icon_dragging = true;
	ax::Point icon_pos(BlockIconWithDesktopBorder(icon, click_pos, mouse));

	std::shared_ptr<DesktopIcon> dIcon(std::static_pointer_cast<DesktopIcon>(icon->backbone));

	_grid_index = FindIconGridIndex(dIcon->GetGridIndex(), icon_pos);

	// Check for overlapping icon.
	for (auto& n : _icons) {
		if (n->GetWindow()->GetId() != icon->GetId()) {
			if (_grid_index == n->GetGridIndex()) {
				_over_lapping_grid_index = _grid_index;
				break;
			}
		}
	}

	if (_over_lapping_grid_index != _grid_index) {
		_over_lapping_grid_index = ax::Point(-1, -1);
	}

	_desktop->GetWindow()->Update();

	icon->dimension.SetPosition(icon_pos);
}

void DesktopExplorer::HandleDraggingReleaseIcon(
	ax::Window::Ptr icon, const ax::Point& click_pos, const ax::Point& mouse)
{
	_has_icon_dragging = false;
	ax::Point icon_pos(BlockIconWithDesktopBorder(icon, click_pos, mouse));

	std::shared_ptr<DesktopIcon> dIcon(std::static_pointer_cast<DesktopIcon>(icon->backbone));

	_grid_index = FindIconGridIndex(dIcon->GetGridIndex(), icon_pos);

	// Check for overlapping icon.
	for (auto& n : _icons) {
		if (n->GetWindow()->GetId() != icon->GetId()) {
			if (_grid_index == n->GetGridIndex()) {
				_grid_index = dIcon->GetGridIndex();
				break;
			}
		}
	}

	_over_lapping_grid_index = ax::Point(-1, -1);
	dIcon->SetGridIndex(_grid_index);

	ax::Rect inside_rect(_desktop->GetWindow()->dimension.GetRect().GetInteriorRect(_interior_delta));

	ax::Point new_icon_pos(inside_rect.position.x + _grid_index.x * _delta_icon + 4,
		inside_rect.position.y + _grid_index.y * _delta_icon + 4);

	icon->dimension.SetPosition(new_icon_pos);
	_desktop->GetWindow()->Update();
}

void DesktopExplorer::HandleMouseLeftDown(const ax::Point& pos)
{
	ax::Point m_pos(pos - _desktop->GetWindow()->dimension.GetAbsoluteRect().position);

	_selection.first = true;
	_selection.second.position = m_pos;
	_selection.second.size = ax::Size(1, 1);

	_desktop->GetWindow()->event.GrabMouse();
	_desktop->GetWindow()->Update();
}

void DesktopExplorer::HandleMouseLeftDragging(const ax::Point& pos)
{
	ax::Point m_pos(pos - _desktop->GetWindow()->dimension.GetAbsoluteRect().position);
	_selection.second.size = m_pos - _selection.second.position;
	_desktop->GetWindow()->Update();
}

void DesktopExplorer::HandleMouseLeftUp(const ax::Point& pos)
{
	if (_desktop->GetWindow()->event.IsGrabbed()) {
		_desktop->GetWindow()->event.UnGrabMouse();
		_selection.first = false;
		_desktop->GetWindow()->Update();
	}
}

ax::Point DesktopExplorer::BlockIconWithDesktopBorder(
	ax::Window::Ptr icon, const ax::Point& click_pos, const ax::Point& mouse)
{
	ax::Point rel_pos(mouse - _desktop->GetWindow()->dimension.GetAbsoluteRect().position);

	ax::Point new_pos(rel_pos - click_pos);

	ax::Rect desk_rect(_desktop->GetWindow()->dimension.GetRect());

	ax::Size win_size(icon->dimension.GetSize());

	// Block icon to desktop border while dragging.
	if (new_pos.x < desk_rect.position.x) {
		new_pos.x = desk_rect.position.x;
	}

	if (new_pos.x + win_size.x > desk_rect.position.x + desk_rect.size.x) {
		new_pos.x = desk_rect.position.x + desk_rect.size.x - win_size.x;
	}

	if (new_pos.y < desk_rect.position.y) {
		new_pos.y = desk_rect.position.y;
	}

	if (new_pos.y + win_size.y > desk_rect.position.y + desk_rect.size.y) {
		new_pos.y = desk_rect.position.y + desk_rect.size.y - win_size.y;
	}

	return new_pos;
}

ax::Point DesktopExplorer::FindIconGridIndex(const ax::Point& past_grid_index, const ax::Point& icon_pos)
{
	ax::Rect inside_rect(_desktop->GetWindow()->dimension.GetRect().GetInteriorRect(_interior_delta));

	int last_j = 0, last_i = 0;

	ax::Point index(past_grid_index);

	for (int y = inside_rect.position.y, j = 0; y < inside_rect.position.y + inside_rect.size.y;
		 y += _delta_icon, j++) {

		last_j = j;

		for (int x = inside_rect.position.x, i = 0; x < inside_rect.position.x + inside_rect.size.x;
			 x += _delta_icon, i++) {

			last_i = i;

			// Block right.
			if (icon_pos.x > inside_rect.position.x + inside_rect.size.x - _delta_icon / 2) {
				index.x = _grid_size.x - 1;
			}

			// Block left.
			if (icon_pos.x < inside_rect.position.x + _delta_icon / 2) {
				index.x = 0;
			}

			// Block top.
			if (icon_pos.y < inside_rect.position.y - _delta_icon / 2) {
				index.y = 0;
			}

			// Block bottom.
			if (icon_pos.y > inside_rect.position.y + inside_rect.size.y - _delta_icon / 2) {
				index.y = _grid_size.y - 1;
			}

			// Find x index.
			if (icon_pos.x > x - _delta_icon / 2 && icon_pos.x < x + _delta_icon) {
				index.x = i;
			}

			// Find y index.
			if (icon_pos.y > y - _delta_icon / 2 && icon_pos.y < y + _delta_icon) {
				index.y = j;
			}
		}
	}

	return index;
}

void DesktopExplorer::SetIconOnGrid(ax::Window::Ptr icon)
{
	std::shared_ptr<DesktopIcon> dIcon(std::static_pointer_cast<DesktopIcon>(icon->backbone));

	ax::Point index = dIcon->GetGridIndex();

	ax::Rect inside_rect(_desktop->GetWindow()->dimension.GetRect().GetInteriorRect(_interior_delta));

	ax::Point new_icon_pos(inside_rect.position.x + index.x * _delta_icon + 4,
		inside_rect.position.y + index.y * _delta_icon + 4);

	icon->dimension.SetPosition(new_icon_pos);
}

void DesktopExplorer::InitDesktopIcon()
{
	// Desktop icons.
	ax::Rect rect(_desktop->GetWindow()->dimension.GetRect());

	int n_icon_per_row = (rect.size.x - 2 * 50) / _delta_icon;
	int x_delta = (rect.size.x - 2 * 50) - n_icon_per_row * _delta_icon;
	int n_icon_per_col = (rect.size.y - 2 * 50) / _delta_icon;
	int y_delta = (rect.size.y - 2 * 50) - n_icon_per_col * _delta_icon;
	_interior_delta = ax::Point(50 + x_delta / 2, 50 + y_delta / 2);

	_grid_size = ax::Size(n_icon_per_row, 10);

	// Read desktop folder.
	ax::os::Directory dir;
	//	dir.Goto("/home/eos/");
	// dir.Goto("/Users/alexarse/Project/eos/platform/mac/desktop/");
	dir.Goto("resources/Desktop");

	int x = 0, y = 0;
	for (auto& n : dir.GetContent()) {
		std::string icon_path;

		if (n.type == ax::os::File::FOLDER) {
			icon_path = "resources/1441952908_folder.png";
		}
		else if (n.ext == "cpp") {
			icon_path = "resources/1441952883_book.png";
		}
		else if (n.ext == "avi" || n.ext == "mp4") {
			icon_path = "resources/1441955471_movie-alt.png";
		}
		else {
			icon_path = "resources/1441952129_document.png";
		}

		std::shared_ptr<eos::DesktopIcon> icon(
			new eos::DesktopIcon(this, ax::Point(50, 50), icon_path, n.name));

		if (x++ > _grid_size.x - 2) {
			x = 0;
			y++;
		}

		_desktop->GetWindow()->node.Add(icon);
		_icons.push_back(icon.get());
		icon->SetGridIndex(ax::Point(x, y));
		SetIconOnGrid(icon->GetWindow());
	}
}

std::vector<DesktopIcon*>& DesktopExplorer::GetIcons()
{
	return _icons;
}

void DesktopExplorer::DrawIcons(ax::GC& gc)
{
	/// TODO : Use DrawPoints instead of drawing points separately.

	if (_has_icon_dragging) {
		gc.SetColor(ax::Color(0.8, 0.4));

		ax::Rect inside_rect(_desktop->GetWindow()->dimension.GetRect().GetInteriorRect(_interior_delta));

		for (int y = inside_rect.position.y, j = 0; y <= inside_rect.position.y + inside_rect.size.y;
			 y += _delta_icon, j++) {
			for (int x = inside_rect.position.x, i = 0; x <= inside_rect.position.x + inside_rect.size.x;
				 x += _delta_icon, i++) {

				if ((i == _grid_index.x && j == _grid_index.y)
					|| (i == _grid_index.x + 1 && j == _grid_index.y)
					|| (i == _grid_index.x && j == _grid_index.y + 1)
					|| (i == _grid_index.x + 1 && j == _grid_index.y + 1)) {

					if (_over_lapping_grid_index == _grid_index) {
						gc.SetColor(ax::Color(1.0f, 0.0f, 0.0f, 0.8f));
					}
					else {
						gc.SetColor(ax::Color(1.0, 0.9));
					}

					gc.DrawPoint(ax::Point(x, y), 5);
					gc.SetColor(ax::Color(0.8, 0.4));
				}
				else {
					gc.DrawPoint(ax::Point(x, y), 5);
				}
			}
		}
	}

	// Selection rectangle.
	if (_selection.first) {
		gc.SetColor(ax::Color(0.7, 0.5));
		gc.DrawRectangle(_selection.second);

		gc.SetColor(ax::Color(0.7, 0.8));
		gc.DrawRectangleContour(_selection.second);
	}
}
} // namespace eos.