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
//_terminal(nullptr),
//_system(system)
{
	win = ax::Window::Create(rect);
	
	win->event.OnPaint = ax::WBind<ax::GC>(this, &Desktop::OnPaint);
	win->event.OnMouseMotion = ax::WBind<ax::Point>(this, &Desktop::OnMouseMotion);
	win->event.OnMouseLeftDown = ax::WBind<ax::Point>(this, &Desktop::OnMouseLeftDown);
	win->event.OnMouseLeftUp = ax::WBind<ax::Point>(this, &Desktop::OnMouseLeftUp);

//	_desktop_apps_status[DSKT_APP_TERMINAL] = false;
//	_desktop_apps_status[DSKT_APP_TRACE]    = false;
//	_desktop_apps_status[DSKT_APP_NOTIFY]   = false;
//	_desktop_apps_status[DSKT_APP_HOME]     = false;
//	_desktop_apps_status[DSKT_APP_VIEWER]   = false;
//	
//	_desktop_apps[DSKT_APP_TERMINAL] = nullptr;
//	_desktop_apps[DSKT_APP_TRACE]    = nullptr;
//	_desktop_apps[DSKT_APP_NOTIFY]   = nullptr;
//	_desktop_apps[DSKT_APP_HOME]     = nullptr;
//	_desktop_apps[DSKT_APP_VIEWER]   = nullptr;
//	
//	_showView = false;
//	
//	_terminal_app = new eos::AppLoader("./app/terminal.so", "TerminalMode");
	
	// Background image.
	_bg_img = std::shared_ptr<ax::Image>(
		new ax::Image("resources/wallpaper-standard.png"));
	
	// Add all icons to desktop.
	// We want to keep the desktop icons at the end of the node children vector
	// to make sure all desktop addons stay on top of icons.
	InitDesktopIcon();
	
	// Create all desktop addons.
	
	// Create status bar.
	win->node.Add(std::shared_ptr<eos::StatusBar>(
			new eos::StatusBar(ax::Rect(0, 0, rect.size.x, 25))));
	
	// Create dock.
	ax::Rect dock_rect(100, rect.size.y - 64 - (2 * 5),
			rect.size.x - 200, 64 + 2 * 5);
	
	win->node.Add(std::shared_ptr<eos::Dock>(
			new eos::Dock(dock_rect)));
	
//	_img_test = new ax::Image("resource/1441953050_image.png");
//	
//	eos::Core::Manager* man = system->GetManager();
//	
//	
//	ax::Rect status_rect(0, 0, rect.size.x, 25);
//	eos::StatusBar* status = new eos::StatusBar(this, status_rect, _system);
//	man->AddChild(status);
//	
//	ax::Rect dock_rect(100, rect.size.y - 64 - (2 * 5),
//					   rect.size.x - 200, 64 + 2 * 5);
//	eos::Dock* dock = new eos::Dock(this, dock_rect);
//	man->AddChild(dock);
	
//	ax::Window* term = _terminal_app->Create(this, ax::Rect(0, 24, rect.size.x, 300));
//	
//	if(term != nullptr)
//	{
//		term->Hide();
//		man->AddChild(term);
//		_desktop_apps[DSKT_APP_TERMINAL] = term;
//	}
//	
//	
//	eos::Notification* notif = new eos::Notification(this,
//													 ax::Rect(0, 24, 200, rect.size.y - 25));
//	notif->Hide();
//	man->AddChild(notif);
//	_desktop_apps[DSKT_APP_NOTIFY] = notif;
//	
//	
//	eos::Alert* trace = new eos::Alert(this,
//									   ax::Rect(rect.size.x - 350, 24, 350, 100),
//									   _system);
//	trace->Hide();
//	man->AddChild(trace);
//	_desktop_apps[DSKT_APP_TRACE] = trace;
//	
//	
//	
//	eos::Home* home = new eos::Home(this, ax::Rect(0, 24, 30, rect.size.y));
//	home->Hide();
//	man->AddChild(home);
//	_desktop_apps[DSKT_APP_HOME] = home;
//	
//	
//	eos::AppViewer* appViewer = new eos::AppViewer(this, ax::Rect(0, 24, 300, rect.size.y));
//	appViewer->Hide();
//	man->AddChild(appViewer);
//	_desktop_apps[DSKT_APP_VIEWER] = appViewer;
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
	dir.Goto("/Users/alexarse/Desktop/");
	
	int x = 0, y = 0;
	for(auto& n : dir.GetContent()) {
		std::string icon_path;
		
		if(n.type == ax::os::File::FOLDER) {
			icon_path = "resources/1441952908_folder.png";
		} else if(n.ext == "cpp") {
			icon_path = "resources/1441952883_book.png";
		} else if(n.ext == "avi" || n.ext == "mp4") {
			icon_path = "resources/1441955471_movie-alt.png";
		} else {
			icon_path = "resources/1441952129_document.png";
		}
		
		std::shared_ptr<eos::DesktopIcon> icon(new eos::DesktopIcon(
				this, ax::Point(50, 50), icon_path, n.name));
		
		if(x++ > _grid_size.x - 2) {
			x = 0;
			y++;
		}
		
		win->node.Add(icon);
		_icons.push_back(icon.get());
		icon->SetGridIndex(ax::Point(x, y));
		SetIconOnGrid(icon->GetWindow());
	}
}

//void eos::Desktop::ShowView()
//{
//	_system->GetTracer()->Write("Desktop view.");
//	
//	_showView = true;
//	
//	eos::Core::Manager* man = _system->GetManager();
//	std::vector<eos::Frame*> frames = man->GetFrameVector();
//	
//	for(auto& n : frames)
//	{
//		if(n->IsShown())
//		{
//			n->AddProperty("ViewMode");
//			n->Hide();
//		}
//	}
//	
//	for(int i = 0; i < DSKT_APP_COUNT; i++)
//	{
//		if(_desktop_apps[i] &&  _desktop_apps_status[i])
//		{
//			_desktop_apps[i]->AddProperty("ViewMode");
//			_desktop_apps[i]->Hide();
//		}
//	}
//	
//	Update();
//}

//void eos::Desktop::ToggleDesktopApp(const DesktopApps& app)
//{
//	if(app < DesktopApps::DSKT_APP_COUNT && _desktop_apps[app])
//	{
//		if(_desktop_apps_status[app])
//		{
//			_desktop_apps[app]->Hide();
//		}
//		else
//		{
//			_desktop_apps[app]->Show();
//		}
//		
//		// Toggle app status.
//		_desktop_apps_status[app] = !_desktop_apps_status[app];
//		
//		ReposAllDesktopBuiltInApps();
//	}
//}

void eos::Desktop::ReposAllDesktopBuiltInApps()
{
//	if(_desktop_apps_status[DSKT_APP_VIEWER])
//	{
//		_desktop_apps[DSKT_APP_NOTIFY]->Hide();
//		_desktop_apps_status[DSKT_APP_NOTIFY] = false;
//	}
//	
//	if(_desktop_apps_status[DSKT_APP_HOME])
//	{
//		_desktop_apps[DSKT_APP_NOTIFY]->SetPosition(ax::Point(29, 24));
//		_desktop_apps[DSKT_APP_VIEWER]->SetPosition(ax::Point(29, 24));
//	}
//	else
//	{
//		ax::Point noti_pos = _desktop_apps[DSKT_APP_NOTIFY]->GetRect().position;
//		
//		if(noti_pos != ax::Point(0, 24))
//		{
//			_desktop_apps[DSKT_APP_NOTIFY]->SetPosition(ax::Point(0, 24));
//		}
//		
//		ax::Point appv_pos = _desktop_apps[DSKT_APP_VIEWER]->GetRect().position;
//		
//		if(appv_pos != ax::Point(0, 24))
//		{
//			_desktop_apps[DSKT_APP_VIEWER]->SetPosition(ax::Point(0, 24));
//		}
//	}
//	
//	if(_desktop_apps_status[DSKT_APP_TERMINAL])
//	{
//		bool c = _desktop_apps_status[DSKT_APP_TRACE];
//		bool n = _desktop_apps_status[DSKT_APP_NOTIFY];
//		bool h = _desktop_apps_status[DSKT_APP_HOME];
//		bool a = _desktop_apps_status[DSKT_APP_VIEWER];
//		
//		
//		ax::Size noti_size = n ? _desktop_apps[DSKT_APP_NOTIFY]->GetSize() : ax::Size(0, 0);
//		ax::Size appv_size = a ? _desktop_apps[DSKT_APP_VIEWER]->GetSize() : ax::Size(0, 0);
//		ax::Size trac_size = c ? _desktop_apps[DSKT_APP_TRACE]->GetSize() : ax::Size(0, 0);
//		ax::Size home_size = h ? _desktop_apps[DSKT_APP_HOME]->GetSize() : ax::Size(0, 0);
//		
//		int sum_size_x = noti_size.x + trac_size.x + home_size.x + appv_size.x;
//		
//		if(h || a)
//		{
//			sum_size_x -= 1;
//		}
//		
//		ax::Size term_size = ax::Size(GetSize().x, 300) - ax::Size(sum_size_x, 0);
//		
//		ax::Point term_pos(appv_size.x + noti_size.x + home_size.x, 24);
//		
//		if(h || a)
//		{
//			term_pos.x -= 1;
//		}
//		
//		if(_desktop_apps[DSKT_APP_TERMINAL])
//		{
//			_desktop_apps[DSKT_APP_TERMINAL]->SetRect(ax::Rect(term_pos, term_size));
//		}
//		
//		if(_desktop_apps[DSKT_APP_TRACE])
//		{
//			_desktop_apps[DSKT_APP_TRACE]->SetSize(ax::Size(350, 300));
//		}
//	}
//	else
//	{
//		if(_desktop_apps_status[DSKT_APP_TRACE] && _desktop_apps[DSKT_APP_TRACE])
//		{
//			ax::Size tr_size(_desktop_apps[DSKT_APP_TRACE]->GetSize());
//			if(tr_size == ax::Size(350, 300))
//			{
//				_desktop_apps[DSKT_APP_TRACE]->SetSize(ax::Size(350, 100));
//			}
//		}
//	}
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

//void eos::Desktop::PaintView(ax::GC& gc)
//{
//	ax::Rect rect(GetDrawingRect());
//	
//	gc.DrawImageResize(_bg_img,
//					   ax::Point(0, 0),
//					   ax::Size(rect.size.x + 1, rect.size.y + 1));
//	
//	gc.SetColor(ax::Color(0.2, 0.2, 0.2, 0.45));
//	gc.DrawRectangle(rect);
//	
//	ax::Point img_pos(50, 50);
//	ax::Size img_size(250, 250);
//	
//	axWindowTree* tree = GetApp()->GetWindowManager()->GetWindowTree();
//	axWindowNode* main_node = tree->FindWinNode(this);
//	
//	eos::Core::Manager* man = _system->GetManager();
//	std::vector<eos::Frame*> frames = man->GetFrameVector();
//	
//	std::function<void(axWindowNode*, ax::Rect, ax::Rect, ax::Point)> fct =
//	[&](axWindowNode* node, ax::Rect main_rect, ax::Rect draw_rect, ax::Point delta_pos)
//	{
//		ax::Window* win = node->window;
//		ax::Rect win_rect = win->GetRect();
//		win_rect.position += delta_pos;
//		
//		ax::Point delta(win_rect.position.x / double(main_rect.size.x) * draw_rect.size.x,
//						win_rect.position.y / double(main_rect.size.y) * draw_rect.size.y);
//		
//		ax::Point pos = draw_rect.position + delta;
//		
//		ax::Size size = ax::Size(win_rect.size.x / double(main_rect.size.x) * draw_rect.size.x,
//								 win_rect.size.y / double(main_rect.size.y) * draw_rect.size.y);
//		
//		gc.DrawTexture(win->GetFrameBuffer()->GetFrameBufferTexture(),
//					   ax::Rect(pos, size));
//		
//		for(auto& k : node->GetChild())
//		{
//			fct(k, main_rect, draw_rect, win_rect.position);
//		}
//	};
//	
//	for(auto& n : frames)
//	{
//		gc.DrawTexture(n->GetFrameBuffer()->GetFrameBufferTexture(),
//					   ax::Rect(img_pos, img_size));
//		
//		
//		for(auto& k : main_node->Get(n)->GetChild())
//		{
//			fct(k,
//				ax::Rect(ax::Point(0, 0), n->GetSize()),
//				ax::Rect(img_pos, img_size),
//				ax::Point(0, 0));
//		}
//		
//		img_pos += ax::Point(img_size.x + 20, 0);
//	}
//}

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

ax::Point eos::Desktop::BlockIconWithDesktopBorder(ax::Window::Ptr icon,
	const ax::Point& click_pos, const ax::Point& mouse)
{
	ax::Point rel_pos(mouse - win->dimension.GetAbsoluteRect().position);
	
	ax::Point new_pos(rel_pos - click_pos);
	
	ax::Rect desk_rect(win->dimension.GetRect());
	
	ax::Size win_size(icon->dimension.GetSize());
	
	// Block icon to desktop border while dragging.
	if(new_pos.x < desk_rect.position.x) {
		new_pos.x = desk_rect.position.x;
	}
	
	if(new_pos.x + win_size.x >
		desk_rect.position.x + desk_rect.size.x) {
		new_pos.x = desk_rect.position.x + desk_rect.size.x -
		win_size.x;
	}
	
	if(new_pos.y < desk_rect.position.y) {
		new_pos.y = desk_rect.position.y;
	}
	
	if(new_pos.y + win_size.y >
		desk_rect.position.y + desk_rect.size.y) {
		new_pos.y = desk_rect.position.y + desk_rect.size.y -
		win_size.y;
	}
	
	return new_pos;
}

ax::Point eos::Desktop::FindIconGridIndex(const ax::Point& past_grid_index,
	const ax::Point& icon_pos)
{
	ax::Rect inside_rect(
		win->dimension.GetRect().GetInteriorRect(_interior_delta));
	
	int last_j = 0, last_i = 0;
	
	ax::Point index(past_grid_index);
	
	for(int y = inside_rect.position.y, j = 0; y <
		inside_rect.position.y + inside_rect.size.y; y += _delta_icon, j++) {
		
		last_j = j;
		
		for(int x = inside_rect.position.x, i = 0; x <
			inside_rect.position.x + inside_rect.size.x; x += _delta_icon, i++) {
			
			last_i = i;
			
			// Block right.
			if(icon_pos.x > inside_rect.position.x +
					inside_rect.size.x - _delta_icon / 2) {
				index.x = _grid_size.x - 1;
			}
			
			// Block left.
			if(icon_pos.x < inside_rect.position.x + _delta_icon / 2) {
				index.x = 0;
			}
			
			// Block top.
			if(icon_pos.y < inside_rect.position.y - _delta_icon / 2) {
				index.y = 0;
			}
			
			// Block bottom.
			if(icon_pos.y > inside_rect.position.y +
					inside_rect.size.y- _delta_icon / 2) {
				index.y = _grid_size.y - 1;
			}
			
			// Find x index.
			if(icon_pos.x > x - _delta_icon / 2 &&
					icon_pos.x < x + _delta_icon) {
				index.x = i;
			}
			
			// Find y index.
			if(icon_pos.y > y - _delta_icon / 2 &&
					icon_pos.y < y + _delta_icon) {
				index.y = j;
			}
		}
	}
	
	return index;
}

void eos::Desktop::HandlePickingIcon(ax::Window::Ptr icon,
	const ax::Point& click_pos, const ax::Point& mouse)
{
	ax::Point icon_pos(BlockIconWithDesktopBorder(icon, click_pos, mouse));
	
	std::shared_ptr<DesktopIcon> dIcon(
		std::static_pointer_cast<DesktopIcon>(icon->backbone));
	
	_grid_index = FindIconGridIndex(dIcon->GetGridIndex(), icon_pos);
	
	win->Update();
}

void eos::Desktop::HandleDraggingIcon(ax::Window::Ptr icon,
	const ax::Point& click_pos, const ax::Point& mouse)
{
	_has_icon_dragging = true;
	ax::Point icon_pos(BlockIconWithDesktopBorder(icon, click_pos, mouse));
	
	std::shared_ptr<DesktopIcon> dIcon(
		std::static_pointer_cast<DesktopIcon>(icon->backbone));
	
	_grid_index = FindIconGridIndex(dIcon->GetGridIndex(), icon_pos);
	
	// Check for overlapping icon.
	for(auto& n : _icons) {
		if(n->GetWindow()->GetId() != icon->GetId()) {
			if(_grid_index == n->GetGridIndex()) {
				_over_lapping_grid_index = _grid_index;
				break;
			}
		}
	}
	
	if(_over_lapping_grid_index != _grid_index) {
		_over_lapping_grid_index = ax::Point(-1, -1);
	}
	
	win->Update();
	
	icon->dimension.SetPosition(icon_pos);
}

void eos::Desktop::HandleDraggingReleaseIcon(ax::Window::Ptr icon,
	const ax::Point& click_pos, const ax::Point& mouse)
{
	_has_icon_dragging = false;
	ax::Point icon_pos(BlockIconWithDesktopBorder(icon, click_pos, mouse));
	
	std::shared_ptr<DesktopIcon> dIcon(
		std::static_pointer_cast<DesktopIcon>(icon->backbone));
	
	_grid_index = FindIconGridIndex(dIcon->GetGridIndex(), icon_pos);
	
	// Check for overlapping icon.
	for(auto& n : _icons) {
		if(n->GetWindow()->GetId() != icon->GetId()) {
			if(_grid_index == n->GetGridIndex()) {
				_grid_index = dIcon->GetGridIndex();
				break;
			}
		}
	}
	
	_over_lapping_grid_index = ax::Point(-1, -1);
	dIcon->SetGridIndex(_grid_index);
	
	ax::Rect inside_rect(
		win->dimension.GetRect().GetInteriorRect(_interior_delta));
	
	ax::Point new_icon_pos(inside_rect.position.x + _grid_index.x * _delta_icon + 4,
		inside_rect.position.y + _grid_index.y * _delta_icon + 4);
	
	icon->dimension.SetPosition(new_icon_pos);
	win->Update();
}

void eos::Desktop::OnPaint(ax::GC gc)
{
	ax::Rect rect(win->dimension.GetDrawingRect());
//
//	if(_showView)
//	{
//		PaintView(gc);
//		return;
//	}
	
	
	gc.DrawImageResize(_bg_img.get(), ax::Point(0, 0),
		ax::Size(rect.size.x + 1, rect.size.y + 1));
	
	if(_has_icon_dragging) {
		gc.SetColor(ax::Color(0.8, 0.4));
		
		ax::Rect inside_rect(
			win->dimension.GetRect().GetInteriorRect(_interior_delta));
		
		for(int y = inside_rect.position.y, j = 0; y <=
			inside_rect.position.y + inside_rect.size.y; y += _delta_icon, j++) {
			for(int x = inside_rect.position.x, i = 0; x <=
				inside_rect.position.x + inside_rect.size.x; x += _delta_icon, i++) {
				
				if((i == _grid_index.x && j == _grid_index.y) ||
					(i == _grid_index.x + 1 && j == _grid_index.y) ||
					(i == _grid_index.x  && j == _grid_index.y + 1) ||
					(i == _grid_index.x + 1 && j == _grid_index.y + 1)) {
					
					if(_over_lapping_grid_index == _grid_index) {
						gc.SetColor(ax::Color(1.0f, 0.0f, 0.0f, 0.8f));
					} else {
						gc.SetColor(ax::Color(1.0, 0.9));
					}
				
					gc.DrawPoint(ax::Point(x, y), 5);
					gc.SetColor(ax::Color(0.8, 0.4));
				} else {
					gc.DrawPoint(ax::Point(x, y), 5);
				}
			}
		}
	}
}
