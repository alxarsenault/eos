#ifndef __EOS_DESKTOP_EXPLORER_H__
#define __EOS_DESKTOP_EXPLORER_H__

#include "axLib/axLib.h"
#include "axLib/axTimer.h"

namespace eos {

class DesktopIcon;
class Desktop;

class DesktopExplorer {
public:
	DesktopExplorer(eos::Desktop* desktop);

	void HandlePickingIcon(ax::Window::Ptr icon, const ax::Point& click_pos,
		const ax::Point& mouse);

	void HandleDraggingIcon(ax::Window::Ptr icon, const ax::Point& click_pos,
		const ax::Point& mouse);

	void HandleDraggingReleaseIcon(ax::Window::Ptr icon,
		const ax::Point& click_pos, const ax::Point& mouse);
	
	void HandleMouseLeftDown(const ax::Point& pos);
	
	void HandleMouseLeftDragging(const ax::Point& pos);
	
	void HandleMouseLeftUp(const ax::Point& pos);

	ax::Point BlockIconWithDesktopBorder(ax::Window::Ptr icon,
		const ax::Point& click_pos, const ax::Point& mouse);

	ax::Point FindIconGridIndex(
		const ax::Point& past_grid_index, const ax::Point& icon_pos);

	void SetIconOnGrid(ax::Window::Ptr icon);

	void InitDesktopIcon();
	
	std::vector<DesktopIcon*>& GetIcons();
	
	void DrawIcons(ax::GC& gc);

private:
	eos::Desktop* _desktop;
	std::vector<DesktopIcon*> _icons;
	std::pair<bool, ax::Rect> _selection;
	
	bool _has_icon_dragging;
	ax::Point _grid_index;
	ax::Point _over_lapping_grid_index;
	ax::Point _interior_delta;
	ax::Size _grid_size;
	
	static const int _delta_icon;
};
}

#endif //__EOS_DESKTOP_EXPLORER_H__
