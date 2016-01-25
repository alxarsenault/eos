#ifndef __EOS_DESKTOP_H__
#define __EOS_DESKTOP_H__

#include <axLib/axLib.h>
#include <axLib/axButton.h>

#include "eosStatusBar.h"
#include "eosDock.h"
#include "eosNotification.h"
#include "eosHome.h"
#include "eosAppViewer.h"

#include "eosFrame.h"
#include "eosSystemCore.h"
#include "eosAppLoader.h"

namespace eos {

class DesktopExplorer;
class DesktopCube;

class Desktop : public ax::Window::Backbone {
public:
	Desktop(const ax::Rect& rect);

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
	
	void ToggleShowApp3DCube();

private:
	std::size_t _last_icon_index;

	int _nDesktopApp;
	
	DesktopExplorer* _explorer;
	ax::Window::Ptr _cube_win;
	ax::Window::Ptr _3D_app_win;
	ax::Window::Ptr _status_bar;

	bool _desktop_apps_status[DSKT_APP_COUNT];
	ax::Window::Ptr _desktop_apps[DSKT_APP_COUNT];

	bool _showView;

	ax::Window* _terminal;
	AppLoader* _terminal_app;

	std::vector<ax::Image::Ptr> _bg_imgs;
	ax::Image* _current_bg_img;

	axEVENT_ACCESSOR(eos::Frame::Msg, OnFrameFullScreen);
	void OnFrameFullScreen(const eos::Frame::Msg& msg);

	void ReposAllDesktopBuiltInApps();

	void ShowIcons(const bool& show);
	void ShowDesktopApps(const bool& show);
	void ShowAppFrames(const bool& show);
	
	void InitMovableDesktopApps(const ax::Rect& rect);
	void InitFixDesktopApps(const ax::Rect& rect);
	
	// ax::Window::Events.
	
	void OnResize(const ax::Size& size);
	
	void OnMouseMotion(const ax::Point& mouse);
	void OnMouseLeftDragging(const ax::Point& mouse);
	void OnMouseLeftDown(const ax::Point& mouse);
	void OnMouseLeftUp(const ax::Point& mouse);
	
	void OnDownArrowDown(const char& c);
	void OnUpArrowDown(const char& c);
	void OnKeyDown(const char& c);

	void OnPaint(ax::GC gc);
	void PaintView(ax::GC& gc);
	
	friend class eos::sys::Core;
};
}

#endif //__EOS_DESKTOP_H__
