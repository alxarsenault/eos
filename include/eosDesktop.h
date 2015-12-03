#ifndef __EOS_DESKTOP_H__
#define __EOS_DESKTOP_H__

#include "axLib/axLib.h"
#include "axLib/axButton.h"
//#include "axLib/axToggle.h"

//#include "eosAppLoader.h"
#include "eosStatusBar.h"
#include "eosDock.h"

//#include "eosFrame.h"
//#include "eosDock.h"
//
#include "eosNotification.h"
//
//#include "eosAlert.h"
#include "eosHome.h"
#include "eosAppViewer.h"

namespace eos {
class DesktopIcon;

class Desktop : public ax::Window::Backbone {
public:
	Desktop(const ax::Rect& rect);

	void HandlePickingIcon(ax::Window::Ptr icon, const ax::Point& click_pos,
		const ax::Point& mouse);

	void HandleDraggingIcon(ax::Window::Ptr icon, const ax::Point& click_pos,
		const ax::Point& mouse);

	void HandleDraggingReleaseIcon(ax::Window::Ptr icon,
		const ax::Point& click_pos, const ax::Point& mouse);

	void AddFrame(std::shared_ptr<ax::Window::Backbone> frame);

	void BringToFront(ax::Window::Ptr frame);

	void ShowView();

	void ShowDesktopChoice();

	enum DesktopApps {
		DSKT_APP_NOTIFY,
		DSKT_APP_TERMINAL,
		DSKT_APP_TRACE,
		DSKT_APP_HOME,
		DSKT_APP_VIEWER,
		DSKT_APP_COUNT
	};

	void ToggleDesktopApp(const DesktopApps& app);

private:
	std::size_t _last_icon_index;

	int _nDesktopApp;
	std::vector<DesktopIcon*> _icons;
	bool _has_icon_dragging;
	ax::Point _grid_index;
	ax::Point _over_lapping_grid_index;
	ax::Point _interior_delta;
	ax::Size _grid_size;
	static const int _delta_icon;

	// Change desktop icon.
	ax::Window::Ptr _cube_win;
	float _cube_angle;
	ax::Point _last_cube_mouse_pos;
	int _cube_face_selected;
	
	// Status bar.
	ax::Window::Ptr _status_bar;

	ax::Point BlockIconWithDesktopBorder(ax::Window::Ptr icon,
		const ax::Point& click_pos, const ax::Point& mouse);

	ax::Point FindIconGridIndex(
		const ax::Point& past_grid_index, const ax::Point& icon_pos);

	void SetIconOnGrid(ax::Window::Ptr icon);

	void InitDesktopIcon();

	bool _desktop_apps_status[DSKT_APP_COUNT];
	ax::Window::Ptr _desktop_apps[DSKT_APP_COUNT];

	bool _showView;

	//		eos::Core::System* _system;

	ax::Window* _terminal;
	AppLoader* _terminal_app;

	std::shared_ptr<ax::Image> _bg_img;
	std::shared_ptr<ax::Image> _bg_img2;
	ax::Image* _current_bg_img;

	axEVENT_ACCESSOR(eos::Frame::Msg, OnFrameFullScreen);
	void OnFrameFullScreen(const eos::Frame::Msg& msg);

	void ReposAllDesktopBuiltInApps();

	void ShowIcons(const bool& show);
	void ShowDesktopApps(const bool& show);
	void ShowAppFrames(const bool& show);
	
	// ax::Window::Events.
	
	void OnMouseMotion(const ax::Point& mouse);
	void OnMouseLeftDown(const ax::Point& mouse);
	void OnMouseLeftUp(const ax::Point& mouse);

	void OnPaint(ax::GC gc);
	void PaintView(ax::GC& gc);
	void OnPaint3D(ax::GC gc);
};
}

#endif //__EOS_DESKTOP_H__
