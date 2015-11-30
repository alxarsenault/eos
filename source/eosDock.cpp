#include "eosDock.h"
#include "eosDockIcon.h"

//#include "eosFrame.h"
//
#include "axLib/axWindowManager.h"
//#include "eosDesktop.h"
//#include "eosAlert.h"

eos::Dock::Dock(const ax::Rect& rect)
	: _anim_percent(1.0)
	, _anim_active(false)
	, _timer_interval(20)
	, _timer_length(200)
	, _anim_up(true)
	, _up_rect(rect)
	, _drop_rect(ax::Rect(
		  rect.position.x, rect.position.y + rect.size.y - 10, rect.size.x, 10))
{
	ax::App& app = ax::App::GetInstance();
	_timer_up
		= new ax::Event::Timer(app.GetEventManager(), GetOnAnimationTimerUp());

	_timer_down = new ax::Event::Timer(
		app.GetEventManager(), GetOnAnimationTimerDown());

	win = ax::Window::Create(rect);
	win->event.OnPaint = ax::WBind<ax::GC>(this, &Dock::OnPaint);
	win->event.OnMouseEnter = ax::WBind<ax::Point>(this, &Dock::OnMouseEnter);
	win->event.OnMouseLeave = ax::WBind<ax::Point>(this, &Dock::OnMouseLeave);
	
	win->event.OnBeforeDrawing = ax::WFunc<int>([&](const int& nothing) {
		if (win->state.Get(ax::Window::StateOption::NeedUpdate)) {
			unsigned char* bg_data = nullptr;
			ax::Rect d_rect = win->GetWindowPixelData(bg_data);
			_bg_img = std::shared_ptr<ax::Image>(new ax::Image(
				bg_data, d_rect.size, ax::Image::ColorType::RGBA));
			delete bg_data;
		}
	});
	
	win->AddConnection(320, GetOnAppSelect());

	ax::Size icon_size(64, 64);

	ax::StringPairVector apps_icon_info
		= { ax::StringPair("resources/1441952929_house.png", "browser"),
			ax::StringPair("resources/1441951759_calculator.png", "calc"),
			ax::StringPair("resources/1441953077_notepad.png", "txtedit"),
			ax::StringPair("resources/1441952725_terminal.png", "term"),
			ax::StringPair("resources/1441953272_enveloppe-alt.png", "mail"),
			ax::StringPair("resources/1441952856_calendar.png", "calender"),
			ax::StringPair("resources/1441952883_book.png", "book"),
			ax::StringPair("resources/1441953050_image.png", "viewer"),
			ax::StringPair(
				"resources/1441953912_wrench-screwdriver.png", "settings"),
			ax::StringPair(
				"resources/1441954538_appicns_Trash_Empty.png", "trash") };

	ax::Point icon_pos(10, 5);

	for (auto& n : apps_icon_info) {

		ax::Window::Ptr icon
			= win->node.Add(std::shared_ptr<eos::DockIcon>(new eos::DockIcon(
				ax::Rect(icon_pos, icon_size), n.first, n.second)));

		_app_icons.push_back(static_cast<eos::DockIcon*>(icon->backbone.get()));

		icon_pos = icon->dimension.GetRect().GetNextPosRight(10);
	}

	_appLoaders["calc"] = AppLoader("./app/calculator.so");
	_appLoaders["browser"] = AppLoader("./app/browser.so");
	_appLoaders["txtedit"] = AppLoader("./app/text_editor.so");
	_appLoaders["term"] = AppLoader("./app/terminal.so");
	_appLoaders["viewer"] = AppLoader("./app/video.so");
	_appLoaders["mail"] = AppLoader("./app/mail.so");
	_appLoaders["calender"] = AppLoader("./app/slideshow.so");
	_appLoaders["book"] = AppLoader("./app/editor.so");
	_appLoaders["settings"] = AppLoader("./app/mail.so");
	_appLoaders["trash"] = AppLoader("./app/mail.so");

	win->dimension.SetRect(_drop_rect);
	_isDrop = true;

	for (auto& n : _app_icons) {
		n->GetWindow()->Hide();
	}

	_shader
		= ax::GL::Shader("img_vertex_shader.glsl", "img_fragments_shader.glsl");

	_shader.CompileAndLink();
}

void eos::Dock::OnAnimationTimerUp(const ax::Event::Timer::Msg& msg)
{
	if (_anim_up == true) {
		int count = (int)msg.GetTime().count() + _timer_interval;
		_anim_percent = float(count) / float(_timer_length);

		// When done going up.
		if (count == _timer_length) {
			_anim_percent = 1.0;
			_anim_active = false;

			// Show all icon on dock.
			for (auto& n : _app_icons) {
				n->GetWindow()->Show();
			}
		}

		win->Update();
	}
}

void eos::Dock::OnAnimationTimerDown(const ax::Event::Timer::Msg& msg)
{
	if (_anim_up == false) {
		int count = (int)msg.GetTime().count() + _timer_interval;
		_anim_percent = float(count) / float(_timer_length);

		// When done going down.
		if (count == _timer_length) {
			_anim_percent = 1.0;
			_anim_active = false;

			win->dimension.SetRect(_drop_rect);

			//			for(auto& n : _app_icons) {
			//				n->GetWindow()->Hide();
			//			}
		}

		win->Update();
	}
}

bool eos::Dock::IsDrop() const
{
	return _isDrop;
}

void eos::Dock::OnMouseEnter(const ax::Point& mouse)
{
	if (_isDrop) {
		ax::Print("Mouse enter.");
		win->dimension.SetRect(_up_rect);

		_isDrop = false;
		_anim_active = true;
		_anim_percent = 0.0;
		_anim_up = true;

		if (_timer_down->IsRunning()) {
			_timer_down->StopTimer();
		}

		_timer_up->StartTimer(
			ax::Event::Timer::TimeMs(_timer_interval), // Interval.
			ax::Event::Timer::TimeMs(_timer_length)); // Length.

		//		// Show all icon on dock.
		//		for(auto& n : _app_icons) {
		//			n->GetWindow()->Show();
		//		}
	}
}

void eos::Dock::OnMouseLeave(const ax::Point& mouse)
{
	bool over = win->dimension.GetAbsoluteRect().IsPointInside(mouse);

	if (over == false) {
		_isDrop = true;
		_anim_active = true;
		_anim_percent = 0.0;
		_anim_up = false;

		if (_timer_up->IsRunning()) {
			_timer_up->StopTimer();
		}

		_timer_down->StartTimer(ax::Event::Timer::TimeMs(_timer_interval),
			ax::Event::Timer::TimeMs(_timer_length));

		// Hide all icons on dock.
		for (auto& n : _app_icons) {
			n->GetWindow()->Hide();
		}
	}
}

void eos::Dock::OnAppSelect(const ax::Event::StringMsg& msg)
{
	ax::Print("App select :", msg.GetMsg());
	AppLoader& loader = _appLoaders[msg.GetMsg()];

	if (loader.GetHandle() == nullptr) {
		ax::Rect rect(500, 50, 162 + 2 * 9, 255 + 25 + 9);

		std::shared_ptr<ax::Window::Backbone> frame(loader.Create(rect));

		if (frame != nullptr) {
			frame->GetWindow()->AddConnection(
				eos::Frame::Events::MINIMIZE, GetOnWindowMinimize());

			frame->GetWindow()->AddConnection(eos::Frame::Events::CLOSE, GetOnWindowClose());

			std::shared_ptr<eos::Desktop> desktop
				= std::static_pointer_cast<eos::Desktop>(
					ax::App::GetInstance().GetTopLevel()->backbone);

			desktop->AddFrame(frame);
//			desktop->GetWindow()->node.Add(frame);
			//			eos::Frame* osframe = static_cast<eos::Frame*>(frame);
			//			static_cast<eos::Desktop*>(GetParent())->AddFrame(osframe);
		}
		else {
			// WARNING MESSAGE BOX.
		}
		return;
	}

	ax::Window::Backbone* frame = loader.GetHandle();

	if (frame != nullptr) {
		if (frame->GetWindow()->IsShown()) {
			frame->GetWindow()->Hide();
		}
		else {
			frame->GetWindow()->Show();
		}
	}
}

void eos::Dock::OnWindowMinimize(const eos::Frame::Msg& msg)
{
	ax::Print("Window minimize.");
	eos::Frame* frame = msg.GetSender();

	if (frame != nullptr) {
		frame->GetWindow()->Hide();
	}
}

void eos::Dock::OnWindowClose(const eos::Frame::Msg& msg)
{
	//ax::Print("Window close.");
//	eos::Frame* frame = msg.GetSender();
//
//	if(frame != nullptr)
//	{
//		for(auto& n : _appLoaders)
//		{
//			ax::Window* handle = n.second.GetHandle();
//
//			if(handle != nullptr)
//			{
//				if(handle->GetId() == frame->GetId())
//				{
//					//ax::Print("Before remove handle.");
//					_appLoaders[n.first].RemoveHandle();
//					frame->DeleteWindow();
//					//ax::Print("Delete before return.");
//					return;
//				}
//			}
//		}
//	}
}

void DrawQuarterCircle(ax::GC gc, const ax::FloatPoint& pos, const int& radius,
	const double& angle, const ax::Color& middle_color,
	const ax::Color& contour_color)
{
	const int& nSegments = 20;

	glBegin(GL_TRIANGLE_FAN);

	gc.SetColor(middle_color);
	glVertex2d(pos.x, pos.y - 1);

	gc.SetColor(contour_color);
	for (int i = 0; i <= nSegments; i++) {
		// Get the current angle.
		double theta = (2.0f * M_PI) * 0.25 * (double(i)) / double(nSegments);

		double x = radius * cosf(theta + angle);
		double y = radius * sinf(theta + angle);

		glVertex2d(x + pos.x, y + pos.y - 1);
	}
	glEnd();
}

void DrawRectangleColorFade(ax::GC gc, const ax::Rect& rectangle,
	const ax::Color& c1, const ax::Color& c2)
{
	ax::FloatRect rect(rectangle.position.x, rectangle.position.y,
		rectangle.size.x, rectangle.size.y);

	glBegin(GL_QUADS);
	{
		gc.SetColor(c1);
		glVertex3f(rect.position.x, rect.position.y, 0); // Bottom left.

		glVertex3f(
			rect.position.x + rect.size.x, rect.position.y, 0); // Bottom Right.

		gc.SetColor(c2);
		glVertex3f(rect.position.x + rect.size.x, rect.position.y + rect.size.y,
			0); // Top Right.

		glVertex3f(
			rect.position.x, rect.position.y + rect.size.y, 0); // Top Left
	}
	glEnd();
}

void DrawRoundedRectangle(ax::GC gc, const ax::Rect& rect, const int& radius,
	const ax::Color& c1, const ax::Color& c2)
{
	int r = radius;

	if (r > rect.size.y * 0.5) {
		r = rect.size.y * 0.5;
	}

	ax::FloatRect frect(
		rect.position.x, rect.position.y, rect.size.x, rect.size.y);

	// Middle.
	DrawRectangleColorFade(gc,
		ax::Rect(frect.position.x + r - 1, frect.position.y - 1,
							   frect.size.x - 2.0 * r + 1, frect.size.y + 1),
		c1, c2);

	int size_rect_height = frect.size.y - 1.0 * r + 1;

	// Left.
	ax::FloatRect lrect(
		frect.position.x - 1, frect.position.y + r - 1, r, size_rect_height);

	// Right.
	ax::FloatRect rrect(frect.position.x + frect.size.x - r,
		frect.position.y + r - 1, r + 1, size_rect_height);

	double color_ratio = 1.0 - lrect.size.y / (double)rect.size.y;

	ax::Color lc1(
		float(c1.GetRed() + (c2.GetRed() - c1.GetRed()) * color_ratio),
		float(c1.GetGreen() + (c2.GetGreen() - c1.GetGreen()) * color_ratio),
		float(c1.GetBlue() + (c2.GetBlue() - c1.GetBlue()) * color_ratio),
		float(c1.GetAlpha() + (c2.GetAlpha() - c1.GetAlpha()) * color_ratio));

	// Draw left rectangle.
	for (int y = lrect.position.y; y <= lrect.position.y + lrect.size.y; y++) {

		double cc_ratio = y / (double)rect.size.y;
		ax::Color cc(
			float(c1.GetRed() + (c2.GetRed() - c1.GetRed()) * cc_ratio),
			float(c1.GetGreen() + (c2.GetGreen() - c1.GetGreen()) * cc_ratio),
			float(c1.GetBlue() + (c2.GetBlue() - c1.GetBlue()) * cc_ratio),
			float(c1.GetAlpha() + (c2.GetAlpha() - c1.GetAlpha()) * cc_ratio));

		// Left rectangle.
		glBegin(GL_LINES);
		gc.SetColor(c1);
		glVertex3f(lrect.position.x, y, 0.0f); // Left.
		gc.SetColor(cc);
		glVertex3f(lrect.position.x + lrect.size.x, y, 0.0f); // Right.
		glEnd();

		// Right rectangle.
		glBegin(GL_LINES);
		gc.SetColor(cc);
		glVertex3f(rrect.position.x, y, 0.0f); // Left.
		gc.SetColor(c1);
		glVertex3f(rrect.position.x + rrect.size.x, y, 0.0f); // Right.
		glEnd();
	}

	// Top left.
	DrawQuarterCircle(gc,
		ax::FloatPoint(frect.position.x + r - 1, frect.position.y + r - 1), r,
		M_PI, lc1, c1);

	// Top right.
	DrawQuarterCircle(gc, ax::FloatPoint(frect.position.x + frect.size.x - r,
							  frect.position.y + r - 1),
		r, 3.0 * M_PI * 0.5, lc1, c1);
}

void eos::Dock::OnPaint(ax::GC gc)
{
	ax::Color dock_color(0.7, 0.3);
	ax::Color dock_contour(1.0, 0.05);

	// Dock is dropped.
	if (_isDrop) {
		// Getting down.
		if (_anim_active) {
			int y_size = 10 + _anim_percent * (_up_rect.size.y - 10);
			ax::Rect dock_rect(0, y_size, _up_rect.size.x, _up_rect.size.y);

			double inv_anim = 1.0 - _anim_percent;
			ax::Color c_contour(dock_contour);
			c_contour.SetAlpha(inv_anim * c_contour.GetAlpha());

			ax::Color c_dock(dock_color);
			c_dock.SetAlpha(inv_anim * dock_color.GetAlpha());

			DrawRoundedRectangle(gc, dock_rect, 25, c_contour, c_dock);
		}
		else { // Static drop position.

			ax::Rect dock_rect(0, 0, _up_rect.size.x, _up_rect.size.y);
			DrawRoundedRectangle(gc, dock_rect, 25, dock_contour, dock_color);
		}
	}
	else {
		// Getting up.
		if (_anim_active) {

			int y_size = 10 + _anim_percent * (_up_rect.size.y - 10);
			int y_pos = _up_rect.size.y - y_size;

			ax::Rect dock_rect(0, y_pos, _up_rect.size.x, _up_rect.size.y);

			ax::Color c_contour(dock_contour);
			c_contour.SetAlpha(_anim_percent * c_contour.GetAlpha());

			ax::Color c_dock(dock_color);
			c_dock.SetAlpha(_anim_percent * dock_color.GetAlpha());

			DrawRoundedRectangle(gc, dock_rect, 25, c_contour, c_dock);
		}
		else { // Static up position.
			ax::Rect dock_rect(0, 0, _up_rect.size.x, _up_rect.size.y);

			if (_bg_img && _bg_img->IsImageReady()) {
				_shader.Activate();
				gc.DrawImage(_bg_img.get(), ax::Point(0, 0), 0.2);
				glUseProgram(0);
			}

			DrawRoundedRectangle(gc, dock_rect, 25, dock_contour, dock_color);
		}
	}
}
