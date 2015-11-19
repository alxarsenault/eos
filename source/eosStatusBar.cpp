#include "eosStatusBar.h"
#include "eosDesktop.h"

eos::StatusBar::StatusBar(const ax::Rect& rect)
	: _font(0)
{
	_shader
		= ax::GL::Shader("img_vertex_shader.glsl", "img_fragments_shader.glsl");

	_shader.CompileAndLink();

	win = ax::Window::Create(rect);

	win->event.OnMouseLeftDown
		= ax::WFunc<ax::Point>([&](const ax::Point& pos) { win->Update(); });

	win->event.OnBeforeDrawing = ax::WFunc<int>([&](const int& nothing) {
		if (win->state.Get(ax::Window::StateOption::NeedUpdate)) {
			unsigned char* bg_data = nullptr;
			ax::Rect d_rect = win->GetWindowPixelData(bg_data);
			_bg_img = std::shared_ptr<ax::Image>(new ax::Image(
				bg_data, d_rect.size, ax::Image::ColorType::RGBA));
			delete bg_data;
		}
	});

	win->event.OnPaint = ax::WBind<ax::GC>(this, &StatusBar::OnPaint);

	_user_name = "Alexandre Arsenault"; //_system->GetUser()->GetFullName();

	_clock = std::shared_ptr<eos::Clock>(
		new eos::Clock(ax::Rect(rect.size.x - 140, 0, 140, 25)));

	win->node.Add(_clock);

	// Transparent button.
	ax::Button::Info btn_info;
	btn_info.normal = ax::Color(0.0, 0.0);
	btn_info.hover = ax::Color(0.0, 0.0);
	btn_info.clicking = ax::Color(0.0, 0.0);
	btn_info.selected = ax::Color(0.0, 0.0);
	btn_info.contour = ax::Color(0.0, 0.0);
	btn_info.font_color = ax::Color(0.0, 0.0);
	btn_info.round_corner_radius = 0;

	// Volume.
	win->node.Add(ax::Button::Ptr(new ax::Button(
		ax::Rect(rect.size.x - 310, 3, 20, 20), ax::Button::Events(), btn_info,
		"resources/volume51.png", "", ax::Button::Flags::SINGLE_IMG)));

	// Home.
	ax::Window::Ptr btn = win->node.Add(ax::Button::Ptr(new ax::Button(
		ax::Rect(6, 3, 20, 20), ax::Event::Bind(this, &StatusBar::OnHome),
		btn_info, "resources/home.png", "", ax::Button::Flags::SINGLE_IMG)));

	ax::Rect btn_rect(
		btn->dimension.GetRect().GetNextPosRight(5), ax::Size(20, 20));

	// Notification.
	btn = win->node.Add(ax::Button::Ptr(new ax::Button(btn_rect,
		ax::Event::Bind(this, &StatusBar::OnNotificationMode), btn_info,
		"resources/sort52.png", "", ax::Button::Flags::SINGLE_IMG)));

	btn_rect.position = btn->dimension.GetRect().GetNextPosRight(5);

	// View.
	btn = win->node.Add(ax::Button::Ptr(new ax::Button(btn_rect,
		ax::Event::Bind(this, &StatusBar::OnAppViewer), btn_info,
		"resources/view.png", "", ax::Button::Flags::SINGLE_IMG)));

	btn_rect.position = btn->dimension.GetRect().GetNextPosRight(5);

	// Terminal.
	btn = win->node.Add(ax::Button::Ptr(new ax::Button(btn_rect,
		ax::Event::Bind(this, &StatusBar::OnTerminalMode), btn_info,
		"resources/terminal_mode.png", "", ax::Button::Flags::SINGLE_IMG)));

	btn_rect.position = btn->dimension.GetRect().GetNextPosRight(5);

	// Settings.
	btn = win->node.Add(ax::Button::Ptr(new ax::Button(btn_rect,
		ax::Event::Bind(this, &StatusBar::OnSettings), btn_info,
		"resources/setting.png", "", ax::Button::Flags::SINGLE_IMG)));

	btn_rect.position = btn->dimension.GetRect().GetNextPosRight(5);

	// Tracer.
	btn = win->node.Add(ax::Button::Ptr(new ax::Button(btn_rect,
		ax::Event::Bind(this, &StatusBar::OnTraceMode), btn_info,
		"resources/list88.png", "", ax::Button::Flags::SINGLE_IMG)));

	btn_rect.position = btn->dimension.GetRect().GetNextPosRight(5);

	// View.
	btn = win->node.Add(ax::Button::Ptr(new ax::Button(btn_rect,
		ax::Event::Bind(this, &StatusBar::OnView), btn_info,
		"resources/elipse.png", "", ax::Button::Flags::SINGLE_IMG)));
}

void eos::StatusBar::OnView(ax::Event::Msg* msg)
{
	//	eos::Desktop* desktop = static_cast<eos::Desktop*>(GetParent());
	//	desktop->ShowView();
}

void eos::StatusBar::OnNotificationMode(ax::Event::Msg* msg)
{
	std::shared_ptr<eos::Desktop> desktop
		= std::static_pointer_cast<eos::Desktop>(
			ax::App::GetInstance().GetTopLevel()->backbone);
	desktop->ToggleDesktopApp(eos::Desktop::DesktopApps::DSKT_APP_NOTIFY);
}

void eos::StatusBar::OnTerminalMode(ax::Event::Msg* msg)
{
	std::shared_ptr<eos::Desktop> desktop
		= std::static_pointer_cast<eos::Desktop>(
			ax::App::GetInstance().GetTopLevel()->backbone);
	desktop->ToggleDesktopApp(eos::Desktop::DesktopApps::DSKT_APP_TERMINAL);
}

void eos::StatusBar::OnTraceMode(ax::Event::Msg* msg)
{
	//	eos::Desktop* desktop = static_cast<eos::Desktop*>(GetParent());
	//	desktop->ToggleDesktopApp(eos::Desktop::DesktopApps::DSKT_APP_TRACE);
}

void eos::StatusBar::OnHome(ax::Event::Msg* msg)
{
	std::shared_ptr<eos::Desktop> desktop
		= std::static_pointer_cast<eos::Desktop>(
			ax::App::GetInstance().GetTopLevel()->backbone);
	desktop->ToggleDesktopApp(eos::Desktop::DesktopApps::DSKT_APP_HOME);
}

void eos::StatusBar::OnAppViewer(ax::Event::Msg* msg)
{
	std::shared_ptr<eos::Desktop> desktop
		= std::static_pointer_cast<eos::Desktop>(
			ax::App::GetInstance().GetTopLevel()->backbone);
	desktop->ToggleDesktopApp(eos::Desktop::DesktopApps::DSKT_APP_VIEWER);
}

void eos::StatusBar::OnSettings(ax::Event::Msg* msg)
{
	//	eos::Desktop* desktop = static_cast<eos::Desktop*>(GetParent());
	//	desktop->ToggleDesktopApp(eos::Desktop::DesktopApps::DSKT_APP_VIEWER);
}

void eos::StatusBar::OnPaint(ax::GC gc)
{
	ax::Rect rect(win->dimension.GetDrawingRect());

	if (_bg_img && _bg_img->IsImageReady()) {
		_shader.Activate();
		GLuint id = _shader.GetProgramId();
		GLint loc = glGetUniformLocation(id, "singleStepOffset");
		if (loc != -1) {
			glUniform1f(loc, 1.0 / float(rect.size.x));
		}
		gc.DrawImage(_bg_img.get(), ax::Point(0, 0), 0.2);
		glUseProgram(0);
	}

	gc.SetColor(ax::Color(0.4, 0.2));
	//	gc.SetColor(ax::Color(0.4f, 0.0f, 0.0f, 1.0));
	gc.DrawRectangle(ax::Rect(0, 0, rect.size.x, rect.size.y));

	gc.SetColor(ax::Color(0.9));
	gc.DrawString(_font, _user_name, ax::Point(rect.size.x - 280, 5));

	//	gc.SetColor(ax::Color(0.4, 0.5));
	//	gc.DrawLine(ax::Point(0, rect.size.y), ax::Point(rect.size.x,
	// rect.size.y));
}
