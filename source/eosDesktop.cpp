#include "eosDesktop.h"
#include "eosCoreTracer.h"
#include "eosStatusBar.h"
#include "eosAlert.h"
#include "eosPlatform.h"

#include "eosDesktopExplorer.h"
#include "eosDesktopCube.h"
#include "eosDesktop3DApp.h"

#include "axLib/axWindowTree.h"
#include "axLib/axWindowManager.h"

#include "eosSystemProxy.h"

#define SHARED_PTR(TYPE, ...) std::shared_ptr<TYPE>(new TYPE(__VA_ARGS__))

eos::Desktop::Desktop(const ax::Rect& rect)
	: _nDesktopApp(0)
	, _showView(false)
{
	win = ax::Window::Create(rect);
	win->event.OnPaint = ax::WBind<ax::GC>(this, &Desktop::OnPaint);
	win->event.OnMouseMotion = ax::WBind<ax::Point>(this, &Desktop::OnMouseMotion);
	win->event.OnMouseLeftDragging = ax::WBind<ax::Point>(this, &Desktop::OnMouseLeftDragging);
	win->event.OnMouseLeftDown = ax::WBind<ax::Point>(this, &Desktop::OnMouseLeftDown);
	win->event.OnMouseLeftUp = ax::WBind<ax::Point>(this, &Desktop::OnMouseLeftUp);
	win->event.OnUpArrowDown = ax::WBind<char>(this, &Desktop::OnUpArrowDown);
	win->event.OnDownArrowDown = ax::WBind<char>(this, &Desktop::OnDownArrowDown);
	win->event.OnResize = ax::WBind<ax::Size>(this, &Desktop::OnResize);

	win->event.GrabGlobalKey();

	// Init app info arrays.
	std::fill_n(_desktop_apps_status, DSKT_APP_COUNT, false);
	std::fill_n(_desktop_apps, DSKT_APP_COUNT, nullptr);

	// Background images.
	_bg_imgs.reserve(2);
	_bg_imgs.push_back(SHARED_PTR(ax::Image, "resources/cat-wallpaper-animals.png"));
	_bg_imgs.push_back(SHARED_PTR(ax::Image, "resources/F18-alpha-wallpaper-standard.png"));
	_current_bg_img = _bg_imgs[0].get();

	// Add all icons to desktop.
	// We want to keep the desktop icons at the beginning of the node children
	// vector to make sure all desktop addons stay on top of icons.
	_explorer = new eos::DesktopExplorer(this);
	_explorer->InitDesktopIcon();

	// All app frames should be added after all icons and before all desktop apps.
	if ((_last_icon_index = win->node.GetChildren().size()) > 0) {
		_last_icon_index--;
	}

	InitMovableDesktopApps(rect);
	InitFixDesktopApps(rect);
}

void eos::Desktop::InitMovableDesktopApps(const ax::Rect& rect)
{
	#define CREATE_DSK_APP(name, rect) win->node.Add(std::shared_ptr<name>(new name(rect)))

	// Create all desktop addons.
	auto create_app = [&](DesktopApps app_index, ax::Window::Ptr dsk_app) {
		dsk_app->Hide();
		_desktop_apps[app_index] = dsk_app;
		_nDesktopApp++;
	};

	// Notification application.
	ax::Rect notif_rect(0, 24, 200, rect.size.y - 25);
	create_app(DSKT_APP_NOTIFY, CREATE_DSK_APP(eos::Notification, notif_rect));

	// Terminal.
	_terminal_app = new eos::AppLoader("./app/terminal.so", "TerminalMode");

	std::shared_ptr<ax::Window::Backbone> term(_terminal_app->Create(ax::Rect(0, 24, rect.size.x, 300)));

	if (term != nullptr) {
		win->node.Add(term);
		term->GetWindow()->Hide();
		_desktop_apps[DSKT_APP_TERMINAL] = term->GetWindow();
		_nDesktopApp++;
	}

	// System trace application.
	ax::Rect trace_rect(0, 24, 30, rect.size.y);
	create_app(DSKT_APP_TRACE, CREATE_DSK_APP(eos::Alert, trace_rect));

	// Home application.
	ax::Rect home_rect(0, 24, 30, rect.size.y);
	create_app(DSKT_APP_HOME, CREATE_DSK_APP(eos::Home, home_rect));

	// Application viewer.
	ax::Rect aviewer_rect(0, 24, 300, rect.size.y);
	create_app(DSKT_APP_VIEWER, CREATE_DSK_APP(eos::AppViewer, aviewer_rect));

	// Cube to change desktop.
	auto cube = SHARED_PTR(DesktopCube, rect);
	cube->SetBackgroundImages(_bg_imgs);
	_cube_win = win->node.Add(cube);
	_cube_win->Hide();

	// DesktopApp3D.
	_3D_app_win = win->node.Add(SHARED_PTR(Desktop3DApp, rect));
	_3D_app_win->Hide();
	
	#undef CREATE_DSK_APP
}

void eos::Desktop::InitFixDesktopApps(const ax::Rect& rect)
{
	#if EOS_PLATFORM_TYPE == EOS_TABLET
	int status_bar_height = 40;
	#else
	int status_bar_height = 25;
	#endif

	// Create status bar.
	ax::Rect status_rect(0, 0, rect.size.x, status_bar_height);
	_status_bar = win->node.Add(SHARED_PTR(eos::StatusBar, status_rect));

	// Create dock.
	ax::Rect dock_rect(100, rect.size.y - 64 - 10, rect.size.x - 200, 64 + 10);
	ax::Window::Ptr dock = win->node.Add(SHARED_PTR(eos::Dock, dock_rect));
	dock->AddConnection(8012, GetOnFrameFullScreen());
}

void eos::Desktop::ShowIcons(const bool& show)
{
	auto& children = win->node.GetChildren();

	if (show) {
		for (int i = 0; i <= _last_icon_index; i++) {
			children[i]->Show();
		}
	}
	else {
		for (int i = 0; i <= _last_icon_index; i++) {
			children[i]->Hide();
		}
	}
}

void eos::Desktop::ShowDesktopApps(const bool& show)
{
	if (show) {
		for (int i = 0; i < DSKT_APP_COUNT; i++) {
			if (_desktop_apps[i] != nullptr && _desktop_apps_status[i]) {
				_desktop_apps[i]->Show();
			}
		}
	}
	else {
		for (int i = 0; i < DSKT_APP_COUNT; i++) {
			if (_desktop_apps[i] != nullptr && _desktop_apps_status[i]) {
				_desktop_apps[i]->Hide();
			}
		}
	}
}

void eos::Desktop::OnFrameFullScreen(const eos::Frame::Msg& msg)
{
	eos::sys::proxy::BringToFront(msg.GetSender()->GetWindow());
	msg.GetSender()->SetFullScreen(ax::Rect(ax::Point(0, 25), win->dimension.GetSize() - ax::Size(0, 25)));
}

void eos::Desktop::ShowAppFrames(const bool& show)
{
	auto& children = win->node.GetChildren();

	unsigned int fdesk_app = _desktop_apps[DesktopApps::DSKT_APP_NOTIFY]->GetId();
	std::size_t fdesk_app_index = 0;

	// Find first desktop app index.
	for (std::size_t i = _last_icon_index; i < children.size(); i++) {
		if (children[i]->GetId() == fdesk_app) {
			fdesk_app_index = i;
			break;
		}
	}

	if (show) {
		for (std::size_t i = _last_icon_index + 1; i < fdesk_app_index; i++) {
			if (children[i]->property.HasProperty("DesktopHidden")) {
				children[i]->property.RemoveProperty("DesktopHidden");
				children[i]->Show();
			}
		}
	}
	else {
		for (std::size_t i = _last_icon_index + 1; i < fdesk_app_index; i++) {
			if (children[i]->IsShown()) {
				children[i]->property.AddProperty("DesktopHidden");
				children[i]->Hide();
			}
		}
	}
}

void eos::Desktop::ShowView()
{
	//	_system->GetTracer()->Write("Desktop view.");

	if (_showView == false) {
		_showView = true;

		if (_cube_win->IsShown()) {
			_cube_win->Hide();
		}
		else {
			ShowIcons(false);
			ShowDesktopApps(false);
			ShowAppFrames(false);
		}
	}
	else {

		ShowIcons(true);
		ShowDesktopApps(true);
		ShowAppFrames(true);

		_showView = false;
	}

	win->Update();
	_status_bar->Update();
}

void eos::Desktop::ToggleShowApp3DCube()
{
	if (_3D_app_win->IsShown()) {
		_3D_app_win->Hide();
		ShowIcons(true);
		ShowDesktopApps(true);
		ShowAppFrames(true);
	}
	else {
		_3D_app_win->Show();
		ShowIcons(false);
		ShowDesktopApps(false);
		ShowAppFrames(false);
	}

	win->Update();
}

void eos::Desktop::ShowDesktopChoice()
{
	if (_cube_win->IsShown()) {
		_cube_win->Hide();

		eos::DesktopCube* cube = static_cast<eos::DesktopCube*>(_cube_win->backbone.get());
		int selected_face = cube->GetFaceSelected();

		if (selected_face == 0) {
			_current_bg_img = _bg_imgs[0].get();
		}
		else if (selected_face == 1) {
			_current_bg_img = _bg_imgs[1].get();
		}

		ShowIcons(true);
		ShowDesktopApps(true);
		ShowAppFrames(true);

		_cube_win->event.UnGrabKey();
	}
	else {
		_cube_win->Show();
		_cube_win->event.GrabKey();
		if (_showView) {
			_showView = false;
		}
		else {
			ShowIcons(false);
			ShowDesktopApps(false);
			ShowAppFrames(false);
		}
	}

	win->Update();
	_status_bar->Update();
}

void eos::Desktop::ToggleDesktopApp(const DesktopApps& app)
{
	if (app < DesktopApps::DSKT_APP_COUNT && _desktop_apps[app]) {
		if (_desktop_apps_status[app]) {
			_desktop_apps[app]->Hide();
		}
		else {
			_desktop_apps[app]->Show();
		}

		// Toggle app status.
		_desktop_apps_status[app] = !_desktop_apps_status[app];

		ReposAllDesktopBuiltInApps();
	}
}

void eos::Desktop::ReposAllDesktopBuiltInApps()
{
	if (_desktop_apps_status[DSKT_APP_VIEWER]) {
		_desktop_apps[DSKT_APP_NOTIFY]->Hide();
		_desktop_apps_status[DSKT_APP_NOTIFY] = false;
	}

	if (_desktop_apps_status[DSKT_APP_HOME]) {
		_desktop_apps[DSKT_APP_NOTIFY]->dimension.SetPosition(ax::Point(29, 24));
		_desktop_apps[DSKT_APP_VIEWER]->dimension.SetPosition(ax::Point(29, 24));
	}
	else {
		ax::Point noti_pos = _desktop_apps[DSKT_APP_NOTIFY]->dimension.GetRect().position;

		if (noti_pos != ax::Point(0, 24)) {
			_desktop_apps[DSKT_APP_NOTIFY]->dimension.SetPosition(ax::Point(0, 24));
		}

		ax::Point appv_pos = _desktop_apps[DSKT_APP_VIEWER]->dimension.GetRect().position;

		if (appv_pos != ax::Point(0, 24)) {
			_desktop_apps[DSKT_APP_VIEWER]->dimension.SetPosition(ax::Point(0, 24));
		}
	}

	if (_desktop_apps_status[DSKT_APP_TERMINAL]) {
		bool c = _desktop_apps_status[DSKT_APP_TRACE];
		bool n = _desktop_apps_status[DSKT_APP_NOTIFY];
		bool h = _desktop_apps_status[DSKT_APP_HOME];
		bool a = _desktop_apps_status[DSKT_APP_VIEWER];

		ax::Size noti_size = n ? _desktop_apps[DSKT_APP_NOTIFY]->dimension.GetSize() : ax::Size(0, 0);
		ax::Size appv_size = a ? _desktop_apps[DSKT_APP_VIEWER]->dimension.GetSize() : ax::Size(0, 0);
		ax::Size trac_size = c ? _desktop_apps[DSKT_APP_TRACE]->dimension.GetSize() : ax::Size(0, 0);
		ax::Size home_size = h ? _desktop_apps[DSKT_APP_HOME]->dimension.GetSize() : ax::Size(0, 0);

		int sum_size_x = noti_size.x + trac_size.x + home_size.x + appv_size.x;

		if (h || a) {
			sum_size_x -= 1;
		}

		ax::Size term_size = ax::Size(win->dimension.GetSize().x, 300) - ax::Size(sum_size_x, 0);

		ax::Point term_pos(appv_size.x + noti_size.x + home_size.x, 24);

		if (h || a) {
			term_pos.x -= 1;
		}

		if (_desktop_apps[DSKT_APP_TERMINAL]) {
			_desktop_apps[DSKT_APP_TERMINAL]->dimension.SetRect(ax::Rect(term_pos, term_size));
		}

		if (_desktop_apps[DSKT_APP_TRACE]) {
			_desktop_apps[DSKT_APP_TRACE]->dimension.SetSize(ax::Size(350, 300));
		}
	}
	else {
		if (_desktop_apps_status[DSKT_APP_TRACE] && _desktop_apps[DSKT_APP_TRACE]) {
			ax::Size tr_size(_desktop_apps[DSKT_APP_TRACE]->dimension.GetSize());
			if (tr_size == ax::Size(350, 300)) {
				_desktop_apps[DSKT_APP_TRACE]->dimension.SetSize(ax::Size(350, 100));
			}
		}
	}
}

void eos::Desktop::OnResize(const ax::Size& size)
{
	win->dimension.SetSize(size);
}

void eos::Desktop::OnMouseMotion(const ax::Point& mouse)
{
}

void eos::Desktop::OnMouseLeftDragging(const ax::Point& mouse)
{
	_explorer->HandleMouseLeftDragging(mouse);
}

void eos::Desktop::OnMouseLeftDown(const ax::Point& mouse)
{
	_explorer->HandleMouseLeftDown(mouse);
	ax::Print("Desktop mouse left down.");

	//	if(_showView)
	//	{
	//		eos::Core::Manager* man = _system->GetManager();
	//		std::vector<eos::Frame*> frames = man->GetFrameVector();
	//
	//		for(auto& n : frames)
	//		{
	//			if(n->HasProperty("ViewMode"))
	//			{
	//				n->Show();
	//				n->RemoveProperty("ViewMode");
	//			}
	//		}
	//
	//		for(int i = 0; i < DSKT_APP_COUNT; i++)
	//		{
	//			if(_desktop_apps[i] &&  _desktop_apps_status[i])
	//			{
	//				if(_desktop_apps[i]->HasProperty("ViewMode"))
	//				{
	//					_desktop_apps[i]->Show();
	//					_desktop_apps[i]->RemoveProperty("ViewMode");
	//				}
	//			}
	//		}
	//
	//		_showView = false;
	//		Update();
	//	}
}

void eos::Desktop::OnMouseLeftUp(const ax::Point& mouse)
{
	_explorer->HandleMouseLeftUp(mouse);
}

void eos::Desktop::OnDownArrowDown(const char& c)
{
	if (ax::App::GetInstance().GetWindowManager()->IsCmdDown()) {
		ShowView();
	}
}

void eos::Desktop::OnUpArrowDown(const char& c)
{
	if (ax::App::GetInstance().GetWindowManager()->IsCmdDown()) {
		ShowDesktopChoice();
	}
}

void eos::Desktop::PaintView(ax::GC& gc)
{
	ax::Rect rect(win->dimension.GetDrawingRect());

	gc.DrawImageResize(_current_bg_img, ax::Point(0, 0), ax::Size(rect.size.x + 1, rect.size.y + 1));

	gc.SetColor(ax::Color(0.15, 0.8));
	gc.DrawRectangle(ax::Rect(ax::Point(0, 0), rect.size + ax::Size(1, 1)));

	ax::Point img_pos(50, 50);
	ax::Size img_size(250, 250);

	const std::vector<ax::Window::Ptr>& frames = win->node.GetChildren();

	std::function<void(ax::Window::Ptr, ax::Rect, ax::Rect, ax::Point)> fct = [&](
		ax::Window::Ptr node_win, ax::Rect main_rect, ax::Rect draw_rect, ax::Point delta_pos) {
		ax::Rect win_rect = win->dimension.GetRect();
		win_rect.position += delta_pos;

		ax::Point delta(win_rect.position.x / double(main_rect.size.x) * draw_rect.size.x,
			win_rect.position.y / double(main_rect.size.y) * draw_rect.size.y);

		ax::Point pos = draw_rect.position + delta;

		ax::Size size = ax::Size(win_rect.size.x / double(main_rect.size.x) * draw_rect.size.x,
			win_rect.size.y / double(main_rect.size.y) * draw_rect.size.y);

		gc.DrawTexture(node_win->dimension.GetFrameBuffer()->GetFrameBufferTexture(), ax::Rect(pos, size));

		//			for (auto& k : node_win->node.GetChildren()) {
		//				// fct(k, main_rect, draw_rect, win_rect.position);
		//			}
	};

	for (auto& n : frames) {

		if (n->property.HasProperty("eosFrame")) {
			gc.DrawTexture(
				n->dimension.GetFrameBuffer()->GetFrameBufferTexture(), ax::Rect(img_pos, img_size));

			const ax::Rect& fRect(n->dimension.GetAbsoluteRect());

			for (auto& k : n->node.GetChildren()) {

				ax::Rect wRect(k->dimension.GetAbsoluteRect());
				wRect.position = wRect.position - fRect.position;

				ax::Point kPos(img_pos.x + wRect.position.x / double(fRect.size.x) * img_size.x,
					img_pos.y + wRect.position.y / double(fRect.size.y) * img_size.y);

				ax::Size kSize(wRect.size.x / double(fRect.size.x) * img_size.x,
					wRect.size.y / double(fRect.size.y) * img_size.y);

				gc.DrawTexture(k->dimension.GetFrameBuffer()->GetFrameBufferTexture(), ax::Rect(kPos, kSize));

				for (auto& w : k->node.GetChildren()) {

					ax::Rect wRect(w->dimension.GetAbsoluteRect());
					wRect.position = wRect.position - fRect.position;

					ax::Point kPos(img_pos.x + wRect.position.x / double(fRect.size.x) * img_size.x,
						img_pos.y + wRect.position.y / double(fRect.size.y) * img_size.y);

					ax::Size kSize(wRect.size.x / double(fRect.size.x) * img_size.x,
						wRect.size.y / double(fRect.size.y) * img_size.y);

					gc.DrawTexture(
						w->dimension.GetFrameBuffer()->GetFrameBufferTexture(), ax::Rect(kPos, kSize));
				}
			}

			img_pos += ax::Point(img_size.x + 20, 0);
		}
	}
}

void eos::Desktop::OnPaint(ax::GC gc)
{
	ax::Rect rect(win->dimension.GetDrawingRect());
	ax::Size f_size(ax::App::GetInstance().GetFrameSize());

	if (_cube_win->IsShown() || _3D_app_win->IsShown()) {
		gc.DrawImageResize(_current_bg_img, ax::Point(0, 0), ax::Size(rect.size.x + 1, rect.size.y + 1));

		gc.SetColor(ax::Color(0.15, 0.8));
		gc.DrawRectangle(ax::Rect(ax::Point(0, 0), rect.size + ax::Size(1, 1)));
		return;
	}

	if (_showView) {
		PaintView(gc);
		return;
	}

	gc.DrawImageResize(_current_bg_img, ax::Point(0, 0), ax::Size(rect.size.x + 1, rect.size.y + 1));

	// Draw desktop icons.
	_explorer->DrawIcons(gc);
}
