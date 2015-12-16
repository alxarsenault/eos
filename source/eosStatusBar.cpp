#include "eosStatusBar.h"
#include "eosDesktop.h"
#include "eosPlatform.h"

#include "eosSystemProxy.h"

eos::StatusBar::StatusBar(const ax::Rect& rect)
	: _font(0)
{
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
		new eos::Clock(ax::Rect(rect.size.x - 140, 0, 140, rect.size.y)));

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

#if EOS_PLATFORM_TYPE == EOS_TABLET
	const ax::Size btn_size(35, 35);
#else
	const ax::Size btn_size(20, 20);
#endif

	// Volume.
	win->node.Add(ax::Button::Ptr(
		new ax::Button(ax::Rect(ax::Point(rect.size.x - 310, 3), btn_size),
			ax::Button::Events(), btn_info, "resources/volume51.png", "",
			ax::Button::Flags::SINGLE_IMG)));

	using AppInfo = std::pair<std::string, ax::Event::Function>;
#define APP_INFO(path, fct_name)                                               \
	AppInfo(path, ax::Event::Bind(this, &StatusBar::fct_name))

	// Status bar icons info.
	std::vector<AppInfo> app_info = { APP_INFO("resources/home.png", OnHome),
		APP_INFO("resources/sort52.png", OnNotificationMode),
		APP_INFO("resources/view.png", OnAppViewer),
		APP_INFO("resources/terminal_mode.png", OnTerminalMode),
		APP_INFO("resources/setting.png", OnSettings),
		APP_INFO("resources/list88.png", OnTraceMode),
		APP_INFO("resources/circles23.png", OnApp3D),
		APP_INFO("resources/elipse.png", OnView) };

	ax::Rect btn_rect(ax::Point(6, 3), btn_size);
	ax::Window::Ptr btn;

	// Create all status bar icons.
	for (auto& n : app_info) {
		btn = win->node.Add(ax::Button::Ptr(new ax::Button(btn_rect, n.second,
			btn_info, n.first, "", ax::Button::Flags::SINGLE_IMG)));

		btn_rect.position = btn->dimension.GetRect().GetNextPosRight(5);
	}
}

void eos::StatusBar::OnView(ax::Event::Msg* msg)
{
	eos::sys::proxy::GetDesktop()->ShowView();
}

void eos::StatusBar::OnApp3D(ax::Event::Msg* msg)
{
	eos::sys::proxy::GetDesktop()->ToggleShowApp3DCube();
}

void eos::StatusBar::OnNotificationMode(ax::Event::Msg* msg)
{
	eos::sys::proxy::GetDesktop()->ToggleDesktopApp(
		eos::Desktop::DesktopApps::DSKT_APP_NOTIFY);
}

void eos::StatusBar::OnTerminalMode(ax::Event::Msg* msg)
{
	eos::sys::proxy::GetDesktop()->ToggleDesktopApp(
		eos::Desktop::DesktopApps::DSKT_APP_TERMINAL);
}

void eos::StatusBar::OnTraceMode(ax::Event::Msg* msg)
{
	eos::sys::proxy::GetDesktop()->ToggleDesktopApp(
		eos::Desktop::DesktopApps::DSKT_APP_TRACE);
}

void eos::StatusBar::OnHome(ax::Event::Msg* msg)
{
	eos::sys::proxy::GetDesktop()->ToggleDesktopApp(
		eos::Desktop::DesktopApps::DSKT_APP_HOME);
}

void eos::StatusBar::OnAppViewer(ax::Event::Msg* msg)
{
	eos::sys::proxy::GetDesktop()->ToggleDesktopApp(
		eos::Desktop::DesktopApps::DSKT_APP_VIEWER);
}

void eos::StatusBar::OnSettings(ax::Event::Msg* msg)
{
	eos::sys::proxy::GetDesktop()->ShowDesktopChoice();
}

void eos::StatusBar::OnPaint(ax::GC gc)
{
	//	ax::Rect rect(win->dimension.GetDrawingRect());
	ax::Rect rect(ax::Point(-1, -1), win->dimension.GetSize());

	// if (_bg_img && _bg_img->IsImageReady()) {
	//	_shader.Activate();
	//	GLuint id = _shader.GetProgramId();
	//	GLint loc = glGetUniformLocation(id, "singleStepOffset");
	//	if (loc != -1) {
	//		glUniform1f(loc, 1.0 / float(rect.size.x));
	//	}
	//	gc.DrawImage(_bg_img.get(), ax::Point(0, 0), 0.2);
	//	glUseProgram(0);
	//}

	gc.SetColor(ax::Color(0.4, 0.2));

	//	gc.SetColor(ax::Color(0.4f, 0.0f, 0.0f, 1.0));
	gc.DrawRectangle(rect);

	gc.SetColor(ax::Color(1.0));
	gc.DrawString(_font, _user_name, ax::Point(rect.size.x - 280, 5));

	//	gc.SetColor(ax::Color(0.4, 0.5));
	//	gc.DrawLine(ax::Point(0, rect.size.y), ax::Point(rect.size.x,
	// rect.size.y));
}
