#include "eosDesktop.h"
#include "eosCoreTracer.h"
#include "eosDesktopIcon.h"
#include "eosStatusBar.h"

#include "axLib/axWindowTree.h"
#include "axLib/axWindowManager.h"
#include "axLib/axOSFileSystem.h"

const int eos::Desktop::_delta_icon = 84;

eos::Desktop::Desktop(const ax::Rect& rect)
	: _has_icon_dragging(false)
	, _grid_index(0, 0)
	, _over_lapping_grid_index(-1, -1)
	, _nDesktopApp(0)
	, _showView(false)
//_terminal(nullptr),
//_system(system)
{
	_cube_angle = 0.0;
	win = ax::Window::Create(rect);

	win->event.OnPaint = ax::WBind<ax::GC>(this, &Desktop::OnPaint);
	win->event.OnMouseMotion
		= ax::WBind<ax::Point>(this, &Desktop::OnMouseMotion);
	win->event.OnMouseLeftDown
		= ax::WBind<ax::Point>(this, &Desktop::OnMouseLeftDown);
	win->event.OnMouseLeftUp
		= ax::WBind<ax::Point>(this, &Desktop::OnMouseLeftUp);

	win->event.GrabGlobalKey();
	win->event.OnUpArrowDown = ax::WFunc<char>([&](const char& c) {

		if (ax::App::GetInstance().GetWindowManager()->IsCmdDown()) {
			ShowDesktopChoice();
		}
	});

	win->event.OnDownArrowDown = ax::WFunc<char>([&](const char& c) {

		if (ax::App::GetInstance().GetWindowManager()->IsCmdDown()) {
			ShowView();
		}
	});
	//	win->event.OnPaint3D = ax::WBind<ax::GC>(this, &Desktop::OnPaint3D);

	//	win->event.OnPaintOverFrameBuffer = ax::WBind<ax::GC>(this,
	//&Desktop::OnPaint3D);

	win->event.OnResize = ax::WFunc<ax::Size>(
		[&](const ax::Size& size) { win->dimension.SetSize(size); });

	_desktop_apps_status[DSKT_APP_TERMINAL] = false;
	_desktop_apps_status[DSKT_APP_TRACE] = false;
	_desktop_apps_status[DSKT_APP_NOTIFY] = false;
	_desktop_apps_status[DSKT_APP_HOME] = false;
	_desktop_apps_status[DSKT_APP_VIEWER] = false;

	_desktop_apps[DSKT_APP_TERMINAL] = nullptr;
	_desktop_apps[DSKT_APP_TRACE] = nullptr;
	_desktop_apps[DSKT_APP_NOTIFY] = nullptr;
	_desktop_apps[DSKT_APP_HOME] = nullptr;
	_desktop_apps[DSKT_APP_VIEWER] = nullptr;

	// Background image.
	// wallpaper-standard.png
	_bg_img = std::shared_ptr<ax::Image>(
		new ax::Image("resources/cat-wallpaper-animals.png"));

	_bg_img2 = std::shared_ptr<ax::Image>(
		new ax::Image("resources/F18-alpha-wallpaper-standard.png"));

	_current_bg_img = _bg_img.get();
	//	_bg_img = std::shared_ptr<ax::Image>(
	//										 new
	// ax::Image("resources/wallpaper-standard.png"));

	// Add all icons to desktop.
	// We want to keep the desktop icons at the beginning of the node children
	// vector to make sure all desktop addons stay on top of icons.
	InitDesktopIcon();
	std::vector<ax::Window::Ptr>& children = win->node.GetChildren();
	_last_icon_index = children.size() - 1;

	// All app frames should be added after all icons and before all desktop
	// apps.

	// Create all desktop addons.
	//--------------------------------------------------------------------------
	ax::Window::Ptr notif(win->node.Add(std::shared_ptr<eos::Notification>(
		new eos::Notification(ax::Rect(0, 24, 200, rect.size.y - 25)))));

	notif->Hide();

	_desktop_apps[DSKT_APP_NOTIFY] = notif;
	_nDesktopApp++;

	// Terminal.
	_terminal_app = new eos::AppLoader("./app/terminal.so", "TerminalMode");

	std::shared_ptr<ax::Window::Backbone> term(
		_terminal_app->Create(ax::Rect(0, 24, rect.size.x, 300)));

	if (term != nullptr) {
		win->node.Add(term);
		term->GetWindow()->Hide();
		_desktop_apps[DSKT_APP_TERMINAL] = term->GetWindow();
		_nDesktopApp++;
	}

	//	eos::Alert* trace = new eos::Alert(this,
	//									   ax::Rect(rect.size.x - 350, 24, 350,
	// 100),
	//									   _system);
	//	trace->Hide();
	//	man->AddChild(trace);
	//	_desktop_apps[DSKT_APP_TRACE] = trace;

	ax::Window::Ptr home(win->node.Add(std::shared_ptr<eos::Home>(
		new eos::Home(ax::Rect(0, 24, 30, rect.size.y)))));
	home->Hide();
	_desktop_apps[DSKT_APP_HOME] = home;
	_nDesktopApp++;

	ax::Window::Ptr appViewer(win->node.Add(std::shared_ptr<eos::AppViewer>(
		new eos::AppViewer(ax::Rect(0, 24, 300, rect.size.y)))));

	appViewer->Hide();

	_desktop_apps[DSKT_APP_VIEWER] = appViewer;
	_nDesktopApp++;

	// Cube to change desktop.
	// This should always be the last child of desktop window.
	_cube_face_selected = 1;

	_cube_win = ax::Window::Create(
		ax::Rect(ax::Point(100, 100), rect.size - ax::Size(200, 200)));
	_cube_win->event.OnPaint3D = ax::WBind<ax::GC>(this, &Desktop::OnPaint3D);

	_cube_win->event.OnRightArrowDown = ax::WFunc<char>([&](const char& c) {

		_cube_angle += 5.0;

		if (_cube_angle > 360.0) {
			_cube_angle -= 360.0;
		}

		if (_cube_angle > 360.0 * 0.25 * 0.5) {
			_cube_face_selected = 1;
		}
		else {
			_cube_face_selected = 0;
		}

		_cube_win->Update();
	});
	
	_cube_win->event.OnLeftArrowDown = ax::WFunc<char>([&](const char& c) {
		
		_cube_angle -= 5.0;
		
		if (_cube_angle < 0.0) {
			_cube_angle += 360.0;
		}
		
		if (_cube_angle > 360.0 * 0.25 * 0.5) {
			_cube_face_selected = 1;
		}
		else {
			_cube_face_selected = 0;
		}
		
		_cube_win->Update();
	});

//	_cube_win->event.OnMouseLeftDown
//		= ax::WFunc<ax::Point>([&](const ax::Point& pos) {
//			_cube_win->event.GrabMouse();
//			_last_cube_mouse_pos = pos;
//		});
//
//	_cube_win->event.OnMouseLeftUp
//		= ax::WFunc<ax::Point>([&](const ax::Point& pos) {
//
//			if (_cube_win->event.IsGrabbed()) {
//				_cube_win->event.UnGrabMouse();
//				_last_cube_mouse_pos = pos;
//			}
//		});
//
//	_cube_win->event.OnMouseLeftDragging
//		= ax::WFunc<ax::Point>([&](const ax::Point& pos) {
//
//			double r = pos.x / double(_last_cube_mouse_pos.x);
//
//			if (r > 1.0) {
//				_cube_angle -= r * 5.0;
//				if (_cube_angle < 0.0) {
//					_cube_angle += 360.0;
//				}
//			}
//			else {
//				_cube_angle += r * 5.0;
//
//				if (_cube_angle > 360.0) {
//					_cube_angle -= 360.0;
//				}
//			}
//
//			if (_cube_angle > 360.0 * 0.25 * 0.5) {
//				_cube_face_selected = 1;
//			}
//			else {
//				_cube_face_selected = 0;
//			}
//
//			_last_cube_mouse_pos = pos;
//			_cube_win->Update();
//		});

	win->node.Add(_cube_win);

	_cube_win->Hide();

	// Create status bar (should be at index : _last_icon_index + 1).
	_status_bar = win->node.Add(std::shared_ptr<eos::StatusBar>(
		new eos::StatusBar(ax::Rect(0, 0, rect.size.x, 25))));

	// Create dock.
	ax::Rect dock_rect(
		100, rect.size.y - 64 - (2 * 5), rect.size.x - 200, 64 + 2 * 5);

	win->node.Add(std::shared_ptr<eos::Dock>(new eos::Dock(dock_rect)));
}

void eos::Desktop::AddFrame(std::shared_ptr<ax::Window::Backbone> frame)
{
	std::vector<ax::Window::Ptr>& children(win->node.GetChildren());

	unsigned int fdesk_app
		= _desktop_apps[DesktopApps::DSKT_APP_NOTIFY]->GetId();
	std::size_t fdesk_app_index = 0;

	// Find first desktop app index.
	for (std::size_t i = _last_icon_index; i < children.size(); i++) {
		if (children[i]->GetId() == fdesk_app) {
			fdesk_app_index = i;
			break;
		}
	}

	// Add frame on top of all frames.
	ax::Window::Ptr child = frame->GetWindow();
	child->node.SetParent(win.get());
	children.insert(children.begin() + fdesk_app_index, child);
	child->backbone = frame;
}

void eos::Desktop::BringToFront(ax::Window::Ptr frame)
{
	if (frame != nullptr) {
		std::vector<ax::Window::Ptr>& children(win->node.GetChildren());

		unsigned int frame_id = frame->GetId();
		std::size_t frame_index = 0;

		// Find frame index.
		for (std::size_t i = _last_icon_index; i < children.size(); i++) {
			if (children[i]->GetId() == frame_id) {
				frame_index = i;
				break;
			}
		}

		// Remove frame from child vector.
		children.erase(children.begin() + frame_index);

		unsigned int fdesk_app
			= _desktop_apps[DesktopApps::DSKT_APP_NOTIFY]->GetId();
		std::size_t fdesk_app_index = 0;

		// Find first desktop app index.
		for (std::size_t i = _last_icon_index; i < children.size(); i++) {
			if (children[i]->GetId() == fdesk_app) {
				fdesk_app_index = i;
				break;
			}
		}

		// Add frame on top of all frames.
		children.insert(children.begin() + fdesk_app_index, frame);
	}
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

void eos::Desktop::ShowAppFrames(const bool& show)
{
	auto& children = win->node.GetChildren();

	unsigned int fdesk_app
		= _desktop_apps[DesktopApps::DSKT_APP_NOTIFY]->GetId();
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

void eos::Desktop::InitDesktopIcon()
{
	// Desktop icons.
	ax::Rect rect(win->dimension.GetRect());

	int n_icon_per_row = (rect.size.x - 2 * 50) / _delta_icon;
	int x_delta = (rect.size.x - 2 * 50) - n_icon_per_row * _delta_icon;
	int n_icon_per_col = (rect.size.y - 2 * 50) / _delta_icon;
	int y_delta = (rect.size.y - 2 * 50) - n_icon_per_col * _delta_icon;
	_interior_delta = ax::Point(50 + x_delta / 2, 50 + y_delta / 2);

	_grid_size = ax::Size(n_icon_per_row, 10);

	// Read desktop folder.
	ax::os::Directory dir;
	//	dir.Goto("/home/eos/");
	dir.Goto("/Users/alexarse/Project/eos/platform/mac/desktop/");

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

		win->node.Add(icon);
		_icons.push_back(icon.get());
		icon->SetGridIndex(ax::Point(x, y));
		SetIconOnGrid(icon->GetWindow());
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

void eos::Desktop::ShowDesktopChoice()
{
	if (_cube_win->IsShown()) {
		_cube_win->Hide();

		if (_cube_face_selected == 0) {
			_current_bg_img = _bg_img.get();
		}
		else if (_cube_face_selected == 1) {
			_current_bg_img = _bg_img2.get();
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
		_desktop_apps[DSKT_APP_NOTIFY]->dimension.SetPosition(
			ax::Point(29, 24));
		_desktop_apps[DSKT_APP_VIEWER]->dimension.SetPosition(
			ax::Point(29, 24));
	}
	else {
		ax::Point noti_pos
			= _desktop_apps[DSKT_APP_NOTIFY]->dimension.GetRect().position;

		if (noti_pos != ax::Point(0, 24)) {
			_desktop_apps[DSKT_APP_NOTIFY]->dimension.SetPosition(
				ax::Point(0, 24));
		}

		ax::Point appv_pos
			= _desktop_apps[DSKT_APP_VIEWER]->dimension.GetRect().position;

		if (appv_pos != ax::Point(0, 24)) {
			_desktop_apps[DSKT_APP_VIEWER]->dimension.SetPosition(
				ax::Point(0, 24));
		}
	}

	if (_desktop_apps_status[DSKT_APP_TERMINAL]) {
		bool c = _desktop_apps_status[DSKT_APP_TRACE];
		bool n = _desktop_apps_status[DSKT_APP_NOTIFY];
		bool h = _desktop_apps_status[DSKT_APP_HOME];
		bool a = _desktop_apps_status[DSKT_APP_VIEWER];

		ax::Size noti_size = n
			? _desktop_apps[DSKT_APP_NOTIFY]->dimension.GetSize()
			: ax::Size(0, 0);
		ax::Size appv_size = a
			? _desktop_apps[DSKT_APP_VIEWER]->dimension.GetSize()
			: ax::Size(0, 0);
		ax::Size trac_size = c
			? _desktop_apps[DSKT_APP_TRACE]->dimension.GetSize()
			: ax::Size(0, 0);
		ax::Size home_size = h
			? _desktop_apps[DSKT_APP_HOME]->dimension.GetSize()
			: ax::Size(0, 0);

		int sum_size_x = noti_size.x + trac_size.x + home_size.x + appv_size.x;

		if (h || a) {
			sum_size_x -= 1;
		}

		ax::Size term_size = ax::Size(win->dimension.GetSize().x, 300)
			- ax::Size(sum_size_x, 0);

		ax::Point term_pos(appv_size.x + noti_size.x + home_size.x, 24);

		if (h || a) {
			term_pos.x -= 1;
		}

		if (_desktop_apps[DSKT_APP_TERMINAL]) {
			_desktop_apps[DSKT_APP_TERMINAL]->dimension.SetRect(
				ax::Rect(term_pos, term_size));
		}

		if (_desktop_apps[DSKT_APP_TRACE]) {
			_desktop_apps[DSKT_APP_TRACE]->dimension.SetSize(
				ax::Size(350, 300));
		}
	}
	else {
		if (_desktop_apps_status[DSKT_APP_TRACE]
			&& _desktop_apps[DSKT_APP_TRACE]) {
			ax::Size tr_size(
				_desktop_apps[DSKT_APP_TRACE]->dimension.GetSize());
			if (tr_size == ax::Size(350, 300)) {
				_desktop_apps[DSKT_APP_TRACE]->dimension.SetSize(
					ax::Size(350, 100));
			}
		}
	}
}

void eos::Desktop::OnMouseMotion(const ax::Point& mouse)
{
}

void eos::Desktop::OnMouseLeftDown(const ax::Point& mouse)
{
	ax::Print("Mouse left down.");

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
}

void eos::Desktop::OnPaint3D(ax::GC gc)
{
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glMatrixMode(GL_PROJECTION); // To operate on the Projection matrix
	glLoadIdentity(); // Reset
	ax::Rect rect(win->dimension.GetDrawingRect());
	double asp_ratio = rect.size.x / (double)rect.size.y;

	gluPerspective(45.0f, asp_ratio, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW); // To operate on model-view matrix

	// Render a color-cube consisting of 6 quads with different colors
	glLoadIdentity(); // Reset the model-view matrix
	glTranslatef(0.0f, 0.0f, -4.0f); // Move right and into the screen
	glRotated(_cube_angle, 0, 1.0, 0.0);

	glEnable(GL_DEPTH_TEST);

	//	glBegin(GL_QUADS);                // Begin drawing the color cube with 6
	// quads
	//	// Top face (y = 1.0f)
	//	glColor3f(0.0f, 1.0f, 0.0f);     // Green
	//	glVertex3f( 1.0f, 1.0f, -1.0f);
	//	glVertex3f(-1.0f, 1.0f, -1.0f);
	//	glVertex3f(-1.0f, 1.0f,  1.0f);
	//	glVertex3f( 1.0f, 1.0f,  1.0f);
	// 	glEnd();

	// 	glBegin(GL_QUADS);
	//	glEnable(GL_TEXTURE_2D);
	//	glBindTexture(GL_TEXTURE_2D, _bg_img->GetTexture());
	//	// Bottom face (y = -1.0f)
	//	glColor3f(1.0f, 0.5f, 0.0f);     // Orange
	//	glTexCoord2d(1.0, 0.0);
	//	glVertex3f( 1.0f, -1.0f,  1.0f);
	//
	//	glTexCoord2d(0.0, 0.0);
	//	glVertex3f(-1.0f, -1.0f,  1.0f);
	//
	//	glTexCoord2d(0.0, 1.0);
	//	glVertex3f(-1.0f, -1.0f, -1.0f);
	//
	//	glTexCoord2d(1.0, 1.0);
	//	glVertex3f( 1.0f, -1.0f, -1.0f);
	//	glDisable(GL_TEXTURE_2D);
	//	glEnd();

	if (_cube_face_selected == 0) {
		glColor4f(1.0f, 0.0f, 0.0f, 0.5f); // Red
		glLineWidth(4.0);

		glBegin(GL_LINES);
		{
			glVertex3f(1.0f, 1.0f, 1.0f);
			glVertex3f(-1.0f, 1.0f, 1.0f);

			glVertex3f(-1.0f, 1.0f, 1.0f);
			glVertex3f(-1.0f, -1.0f, 1.0f);

			glVertex3f(-1.0f, -1.0f, 1.0f);
			glVertex3f(1.0f, -1.0f, 1.0f);

			glVertex3f(1.0f, -1.0f, 1.0f);
			glVertex3f(1.0f, 1.0f, 1.0f);
		}
		glEnd();

		glLineWidth(1.0);
	}

	if (_cube_face_selected == 1) {
		glColor4f(1.0f, 0.0f, 0.0f, 0.5f); // Red
		glLineWidth(4.0);

		glBegin(GL_LINES);
		{
			glVertex3f(-1.0f, 1.0f, 1.0f);
			glVertex3f(-1.0f, 1.0f, -1.0f);

			glVertex3f(-1.0f, 1.0f, -1.0f);
			glVertex3f(-1.0f, -1.0f, -1.0f);

			glVertex3f(-1.0f, -1.0f, -1.0f);
			glVertex3f(-1.0f, -1.0f, 1.0f);

			glVertex3f(-1.0f, -1.0f, 1.0f);
			glVertex3f(-1.0f, 1.0f, 1.0f);
		}
		glEnd();

		glLineWidth(1.0);
	}

	// Front face  (z = 1.0f)
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _bg_img->GetTexture());
	glBegin(GL_QUADS);
	glColor3f(1.0f, 0.0f, 0.0f); // Red
	glTexCoord2d(1.0, 0.0);
	glVertex3f(1.0f, 1.0f, 1.0f);

	glTexCoord2d(0.0, 0.0);
	glVertex3f(-1.0f, 1.0f, 1.0f);

	glTexCoord2d(0.0, 1.0);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glTexCoord2d(1.0, 1.0);
	glVertex3f(1.0f, -1.0f, 1.0f);
	glDisable(GL_TEXTURE_2D);
	glEnd();

	//	// Back face (z = -1.0f)
	//	glColor3f(1.0f, 1.0f, 0.0f);     // Yellow
	//	glVertex3f( 1.0f, -1.0f, -1.0f);
	//	glVertex3f(-1.0f, -1.0f, -1.0f);
	//	glVertex3f(-1.0f,  1.0f, -1.0f);
	//	glVertex3f( 1.0f,  1.0f, -1.0f);
	// 	glEnd();  // End of drawing color-cube
	//

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _bg_img2->GetTexture());
	glBegin(GL_QUADS);
	// Left face (x = -1.0f)
	glColor3f(0.0f, 0.0f, 1.0f); // Blue

	glTexCoord2d(1.0, 0.0);
	glVertex3f(-1.0f, 1.0f, 1.0f);

	glTexCoord2d(0.0, 0.0);
	glVertex3f(-1.0f, 1.0f, -1.0f);

	glTexCoord2d(0.0, 1.0);
	glVertex3f(-1.0f, -1.0f, -1.0f);

	glTexCoord2d(1.0, 1.0);
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	//
	// 	glBegin(GL_QUADS);
	//	// Right face (x = 1.0f)
	//	glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
	//	glVertex3f(1.0f,  1.0f, -1.0f);
	//	glVertex3f(1.0f,  1.0f,  1.0f);
	//	glVertex3f(1.0f, -1.0f,  1.0f);
	//	glVertex3f(1.0f, -1.0f, -1.0f);
	//	glEnd();  // End of drawing color-cube

	glDisable(GL_CULL_FACE);
}

void eos::Desktop::PaintView(ax::GC& gc)
{
	ax::Rect rect(win->dimension.GetDrawingRect());

	gc.DrawImageResize(_current_bg_img, ax::Point(0, 0),
		ax::Size(rect.size.x + 1, rect.size.y + 1));

	gc.SetColor(ax::Color(0.05, 0.95));
	gc.DrawRectangle(ax::Rect(ax::Point(0, 0), rect.size + ax::Size(1, 1)));

	ax::Point img_pos(50, 50);
	ax::Size img_size(250, 250);

	const std::vector<ax::Window::Ptr>& frames = win->node.GetChildren();

	std::function<void(ax::Window::Ptr, ax::Rect, ax::Rect, ax::Point)> fct =
		[&](ax::Window::Ptr node_win, ax::Rect main_rect, ax::Rect draw_rect,
			ax::Point delta_pos) {
			ax::Rect win_rect = win->dimension.GetRect();
			win_rect.position += delta_pos;

			ax::Point delta(win_rect.position.x / double(main_rect.size.x)
					* draw_rect.size.x,
				win_rect.position.y / double(main_rect.size.y)
					* draw_rect.size.y);

			ax::Point pos = draw_rect.position + delta;

			ax::Size size = ax::Size(
				win_rect.size.x / double(main_rect.size.x) * draw_rect.size.x,
				win_rect.size.y / double(main_rect.size.y) * draw_rect.size.y);

			gc.DrawTexture(
				node_win->dimension.GetFrameBuffer()->GetFrameBufferTexture(),
				ax::Rect(pos, size));

			//			for (auto& k : node_win->node.GetChildren()) {
			//				// fct(k, main_rect, draw_rect, win_rect.position);
			//			}
		};

	for (auto& n : frames) {

		if (n->property.HasProperty("eosFrame")) {
			gc.DrawTexture(
				n->dimension.GetFrameBuffer()->GetFrameBufferTexture(),
				ax::Rect(img_pos, img_size));

			const ax::Rect& fRect(n->dimension.GetAbsoluteRect());

			for (auto& k : n->node.GetChildren()) {

				ax::Rect wRect(k->dimension.GetAbsoluteRect());
				wRect.position = wRect.position - fRect.position;

				ax::Point kPos(img_pos.x
						+ wRect.position.x / double(fRect.size.x) * img_size.x,
					img_pos.y
						+ wRect.position.y / double(fRect.size.y) * img_size.y);

				ax::Size kSize(wRect.size.x / double(fRect.size.x) * img_size.x,
					wRect.size.y / double(fRect.size.y) * img_size.y);

				gc.DrawTexture(
					k->dimension.GetFrameBuffer()->GetFrameBufferTexture(),
					ax::Rect(kPos, kSize));

				for (auto& w : k->node.GetChildren()) {

					ax::Rect wRect(w->dimension.GetAbsoluteRect());
					wRect.position = wRect.position - fRect.position;

					ax::Point kPos(img_pos.x
							+ wRect.position.x / double(fRect.size.x)
								* img_size.x,
						img_pos.y
							+ wRect.position.y / double(fRect.size.y)
								* img_size.y);

					ax::Size kSize(
						wRect.size.x / double(fRect.size.x) * img_size.x,
						wRect.size.y / double(fRect.size.y) * img_size.y);

					gc.DrawTexture(
						w->dimension.GetFrameBuffer()->GetFrameBufferTexture(),
						ax::Rect(kPos, kSize));
				}
			}

			img_pos += ax::Point(img_size.x + 20, 0);
		}
	}
}

void eos::Desktop::SetIconOnGrid(ax::Window::Ptr icon)
{
	std::shared_ptr<DesktopIcon> dIcon(
		std::static_pointer_cast<DesktopIcon>(icon->backbone));

	ax::Point index = dIcon->GetGridIndex();

	ax::Rect inside_rect(
		win->dimension.GetRect().GetInteriorRect(_interior_delta));

	ax::Point new_icon_pos(inside_rect.position.x + index.x * _delta_icon + 4,
		inside_rect.position.y + index.y * _delta_icon + 4);

	icon->dimension.SetPosition(new_icon_pos);
}

ax::Point eos::Desktop::BlockIconWithDesktopBorder(
	ax::Window::Ptr icon, const ax::Point& click_pos, const ax::Point& mouse)
{
	ax::Point rel_pos(mouse - win->dimension.GetAbsoluteRect().position);

	ax::Point new_pos(rel_pos - click_pos);

	ax::Rect desk_rect(win->dimension.GetRect());

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

ax::Point eos::Desktop::FindIconGridIndex(
	const ax::Point& past_grid_index, const ax::Point& icon_pos)
{
	ax::Rect inside_rect(
		win->dimension.GetRect().GetInteriorRect(_interior_delta));

	int last_j = 0, last_i = 0;

	ax::Point index(past_grid_index);

	for (int y = inside_rect.position.y, j = 0;
		 y < inside_rect.position.y + inside_rect.size.y;
		 y += _delta_icon, j++) {

		last_j = j;

		for (int x = inside_rect.position.x, i = 0;
			 x < inside_rect.position.x + inside_rect.size.x;
			 x += _delta_icon, i++) {

			last_i = i;

			// Block right.
			if (icon_pos.x > inside_rect.position.x + inside_rect.size.x
					- _delta_icon / 2) {
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
			if (icon_pos.y > inside_rect.position.y + inside_rect.size.y
					- _delta_icon / 2) {
				index.y = _grid_size.y - 1;
			}

			// Find x index.
			if (icon_pos.x > x - _delta_icon / 2
				&& icon_pos.x < x + _delta_icon) {
				index.x = i;
			}

			// Find y index.
			if (icon_pos.y > y - _delta_icon / 2
				&& icon_pos.y < y + _delta_icon) {
				index.y = j;
			}
		}
	}

	return index;
}

void eos::Desktop::HandlePickingIcon(
	ax::Window::Ptr icon, const ax::Point& click_pos, const ax::Point& mouse)
{
	ax::Point icon_pos(BlockIconWithDesktopBorder(icon, click_pos, mouse));

	std::shared_ptr<DesktopIcon> dIcon(
		std::static_pointer_cast<DesktopIcon>(icon->backbone));

	_grid_index = FindIconGridIndex(dIcon->GetGridIndex(), icon_pos);

	win->Update();
}

void eos::Desktop::HandleDraggingIcon(
	ax::Window::Ptr icon, const ax::Point& click_pos, const ax::Point& mouse)
{
	_has_icon_dragging = true;
	ax::Point icon_pos(BlockIconWithDesktopBorder(icon, click_pos, mouse));

	std::shared_ptr<DesktopIcon> dIcon(
		std::static_pointer_cast<DesktopIcon>(icon->backbone));

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

	win->Update();

	icon->dimension.SetPosition(icon_pos);
}

void eos::Desktop::HandleDraggingReleaseIcon(
	ax::Window::Ptr icon, const ax::Point& click_pos, const ax::Point& mouse)
{
	_has_icon_dragging = false;
	ax::Point icon_pos(BlockIconWithDesktopBorder(icon, click_pos, mouse));

	std::shared_ptr<DesktopIcon> dIcon(
		std::static_pointer_cast<DesktopIcon>(icon->backbone));

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

	ax::Rect inside_rect(
		win->dimension.GetRect().GetInteriorRect(_interior_delta));

	ax::Point new_icon_pos(
		inside_rect.position.x + _grid_index.x * _delta_icon + 4,
		inside_rect.position.y + _grid_index.y * _delta_icon + 4);

	icon->dimension.SetPosition(new_icon_pos);
	win->Update();
}

void eos::Desktop::OnPaint(ax::GC gc)
{
	ax::Rect rect(win->dimension.GetDrawingRect());

	ax::Print("Rect :", rect.size.x, rect.size.y);
	ax::Size f_size(ax::App::GetInstance().GetFrameSize());
	ax::Print("Frame :", f_size.x, f_size.y);

	if (_cube_win->IsShown()) {
		gc.DrawImageResize(_current_bg_img, ax::Point(0, 0),
			ax::Size(rect.size.x + 1, rect.size.y + 1));

		gc.SetColor(ax::Color(0.05, 0.95));
		gc.DrawRectangle(ax::Rect(ax::Point(0, 0), rect.size + ax::Size(1, 1)));
		return;
	}

	if (_showView) {
		PaintView(gc);
		return;
	}

	gc.DrawImageResize(_current_bg_img, ax::Point(0, 0),
		ax::Size(rect.size.x + 1, rect.size.y + 1));

	if (_has_icon_dragging) {
		gc.SetColor(ax::Color(0.8, 0.4));

		ax::Rect inside_rect(
			win->dimension.GetRect().GetInteriorRect(_interior_delta));

		for (int y = inside_rect.position.y, j = 0;
			 y <= inside_rect.position.y + inside_rect.size.y;
			 y += _delta_icon, j++) {
			for (int x = inside_rect.position.x, i = 0;
				 x <= inside_rect.position.x + inside_rect.size.x;
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
}
