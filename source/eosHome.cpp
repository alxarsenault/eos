#include "eosHome.h"
#include "eosDesktop.h"

eos::Home::Home(const ax::Rect& rect)
{
	_shader
	= ax::GL::Shader("img_vertex_shader.glsl", "img_fragments_shader.glsl");
	
	_shader.CompileAndLink();
	
	win = ax::Window::Create(rect);
	
	win->event.OnBeforeDrawing = ax::WFunc<int>([&](const int& nothing) {
		if (win->state.Get(ax::Window::StateOption::NeedUpdate)) {
			unsigned char* bg_data = nullptr;
			ax::Rect d_rect = win->GetWindowPixelData(bg_data);
			_bg_img = std::shared_ptr<ax::Image>(new ax::Image(
															   bg_data, d_rect.size, ax::Image::ColorType::RGBA));
			delete bg_data;
		}
	});
	
	win->event.OnPaint = ax::WBind<ax::GC>(this, &Home::OnPaint);
	
	ax::Button::Info btn_info(ax::Color(0.5f, 0.5f, 0.5f, 0.0f),
		ax::Color(1.0f, 0.0f, 0.0f, 0.0f), ax::Color(0.95, 0.0, 0.0, 0.0f),
		ax::Color(0.5f, 0.5f, 0.5f, 0.0f), ax::Color(0.0f, 0.0f, 0.0f, 0.0f),
		ax::Color(0.0f, 0.0f, 0.0f, 0.0f), 0);

	std::vector<std::string> btns = { std::string("resources/screen44.png"),
		std::string("resources/shared1.png"), std::string("resources/work3.png"),
		std::string("resources/tack.png"),
		std::string("resources/mountain31.png"),
		std::string("resources/film61.png"),
		std::string("resources/graduate32.png"),
		std::string("resources/black402.png"),
		std::string("resources/favorite21.png"),
		std::string("resources/facebook56.png"),
		std::string("resources/dark55.png"),
		std::string("resources/clipboard99.png"),
		std::string("resources/attachment19.png") };

	ax::Size btnSize(20, 20);
	ax::Point btnPos(6, 5);

	for (auto& n : btns) {
		ax::Window::Ptr btn = win->node.Add(std::shared_ptr<ax::Button>(
			new ax::Button(ax::Rect(btnPos, btnSize), ax::Button::Events(),
				btn_info, n, "", ax::Button::Flags::SINGLE_IMG)));

		btnPos = btn->dimension.GetRect().GetNextPosDown(5);
	}
}

void eos::Home::OnPaint(ax::GC gc)
{
	ax::Rect rect(win->dimension.GetDrawingRect());

	if (_bg_img && _bg_img->IsImageReady()) {
		_shader.Activate();
		GLuint id = _shader.GetProgramId();
		GLint loc = glGetUniformLocation(id, "singleStepOffset");
		if (loc != -1) {
   			glUniform1f(loc, 1.0 / float(rect.size.y));
		}
		
		gc.DrawImage(_bg_img.get(), ax::Point(0, 0), 0.2);
		glUseProgram(0);
	}
	
	gc.SetColor(ax::Color(0.4, 0.2));
	//	gc.SetColor(ax::Color(0.4f, 0.0f, 0.0f, 1.0));
	gc.DrawRectangle(ax::Rect(0, 0, rect.size.x, rect.size.y));
//
//	gc.SetColor(ax::Color(0.4, 0.4));
//	gc.DrawRectangle(ax::Rect(0, 0, rect.size.x, rect.size.y));
//
//	gc.SetColor(ax::Color(0.4, 0.5));
//	gc.DrawRectangleContour(ax::Rect(1, 0, rect.size.x, rect.size.y));
}
