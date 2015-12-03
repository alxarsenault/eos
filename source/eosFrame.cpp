#include "eosFrame.h"
#include "eosDesktop.h"

const int eos::Frame::_title_bar_height = 25;

/*******************************************************************************
 * eos::Frame::Msg
 ******************************************************************************/
eos::Frame::Msg::Msg()
	: _sender(nullptr)
{
}

eos::Frame::Msg::Msg(Frame* sender)
	: _sender(sender)
{
}

eos::Frame* eos::Frame::Msg::GetSender() const
{
	return _sender;
}

ax::Event::Msg* eos::Frame::Msg::GetCopy()
{
	return new eos::Frame::Msg(*this);
}

/*******************************************************************************
 * eos::Frame.
 ******************************************************************************/
eos::Frame::Frame(const ax::Rect& rect, const std::string& window_name)
	: _font(0)
	, _child(nullptr)
	, _child_menu(nullptr)
	, _window_name(window_name)
	, _menuMode(false)
{
	_bg_img = nullptr;
	_abs_click_pos = ax::Point(0, 0);
	_frame_status = 0;
	_highlight = false;

	//_shader
	//	= ax::GL::Shader("img_vertex_shader.glsl", "img_fragments_shader.glsl");
	//_shader.CompileAndLink();

	//_shadow_shader = ax::GL::Shader(
	//	"shadow_vertex_shader.glsl", "shadow_fragments_shader.glsl");
	//_shadow_shader.CompileAndLink();

	win = ax::Window::Create(rect);

	win->property.AddProperty("eosFrame");

	win->event.OnBeforeDrawing = ax::WFunc<int>([&](const int& nothing) {
		if (win->state.Get(ax::Window::StateOption::NeedUpdate)) {
			unsigned char* bg_data = nullptr;
			ax::Rect r(win->dimension.GetDrawingRect());
			ax::Rect rect(r.position, r.size);
			ax::Rect topRect(
				rect.position, ax::Size(rect.size.x, _title_bar_height));
			win->GetWindowPixelData(bg_data, topRect);

			if (_bg_img) {
				delete _bg_img;
			}
			_bg_img = new ax::Image(
				bg_data, topRect.size, ax::Image::ColorType::RGBA);
			//			_bg_img = std::shared_ptr<ax::Image>(new ax::Image(
			//				bg_data, topRect.size, ax::Image::ColorType::RGBA));
			delete bg_data;
		}
	});

	//win->event.OnPaintOverFrameBuffer
	//	= ax::WBind<ax::GC>(this, &Frame::OnPaintOverFrameBuffer);
	
	win->event.OnPaint = ax::WBind<ax::GC>(this, &Frame::OnPaint);
	win->event.OnMouseLeave = ax::WBind<ax::Point>(this, &Frame::OnMouseLeave);
	win->event.OnMouseMotion
		= ax::WBind<ax::Point>(this, &Frame::OnMouseMotion);
	win->event.OnMouseLeftDown
		= ax::WBind<ax::Point>(this, &Frame::OnMouseLeftDown);
	win->event.OnMouseLeftUp
		= ax::WBind<ax::Point>(this, &Frame::OnMouseLeftUp);
	win->event.OnMouseLeftDragging
		= ax::WBind<ax::Point>(this, &Frame::OnMouseLeftDragging);

	ax::Button::Info menu_btn_info(ax::Color(0.5f, 0.5f, 0.5f, 0.0f),
		ax::Color(1.0f, 0.0f, 0.0f, 0.0f), ax::Color(0.95f, 0.0f, 0.0f, 0.0f),
		ax::Color(0.5f, 0.5f, 0.5f, 0.0f), ax::Color(0.0f, 0.0f, 0.0f, 0.0f),
		ax::Color(0.0f, 0.0f, 0.0f, 0.0f), 0);

	//    _info_btn = new ax::Button(this, ax::Rect(50, 50, 48, 48),
	//                               ax::Button::Events(),
	//                               menu_btn_info,
	//                               "resource/info.png", "",
	//                               ax::Button::Flags::SINGLE_IMG);

	//    _info_btn->Hide();

	_menu_btn = ax::Button::Ptr(new ax::Button(ax::Rect(10, 4, 20, 20),
		GetOnOpenMenu(), menu_btn_info, "resources/show6.png", ""));

	win->node.Add(_menu_btn);

	ax::Button::Info btn_info(ax::Color(0.5f, 0.5f, 0.5f, 0.0f),
		ax::Color(1.0f, 0.0f, 0.0f), ax::Color(0.95f, 0.0f, 0.0f),
		ax::Color(0.5f, 0.5f, 0.5f, 0.0f), ax::Color(0.0f, 0.0f, 0.0f, 0.0f),
		ax::Color(0.9), 0);

	_close_btn = ax::Button::Ptr(
		new ax::Button(ax::Rect(rect.size.x - 25 - 5 - 1, 0, 23, 25),
			ax::Button::Events(GetOnButtonClick()), btn_info, "", "x"));

	win->node.Add(_close_btn);

	btn_info.hover = ax::Color(0.0f, 0.0f, 1.0f);
	btn_info.clicking = ax::Color(0.0f, 0.0f, 0.95f);

	_min_btn = ax::Button::Ptr(
		new ax::Button(ax::Rect(rect.size.x - 25 - 5 - 1 - 25, 0, 23, 25),
			ax::Button::Events(GetOnMinimize()), btn_info, "", "_"));

	win->node.Add(_min_btn);
}

// eos::Frame::Frame(
//	ax::App* app, const ax::Rect& rect, const std::string& window_name)
//	: // Parent.
//	axPanel(app, rect)
//	, _font(0)
//	, _child(nullptr)
//	, _child_menu(nullptr)
//	, _window_name(window_name)
//	, _menuMode(false)
//{
//	_abs_click_pos = ax::Point(0, 0);
//	_frame_status = 0;
//	_highlight = false;
//
//	ax::Button::Info menu_btn_info(ax::Color(0.5, 0.5, 0.5, 0.0),
//		ax::Color(1.0, 0.0, 0.0, 0.0), ax::Color(0.95, 0.0, 0.0, 0.0),
//		ax::Color(0.5, 0.5, 0.5, 0.0), ax::Color(0.0, 0.0, 0.0, 0.0),
//		ax::Color(0.0, 0.0, 0.0, 0.0), 0);
//
//	//    _info_btn = new ax::Button(this, ax::Rect(50, 50, 48, 48),
//	//                               ax::Button::Events(),
//	//                               menu_btn_info,
//	//                               "resource/info.png", "",
//	//                               ax::Button::Flags::SINGLE_IMG);
//
//	//    _info_btn->Hide();
//
//	_menu_btn = new ax::Button(this, ax::Rect(10, 4, 20, 20), GetOnOpenMenu(),
//		menu_btn_info, "resource/show6.png", "");
//
//	ax::Button::Info btn_info(ax::Color(0.5, 0.5, 0.5, 0.0),
//		ax::Color(1.0, 0.0, 0.0), ax::Color(0.95, 0.0, 0.0),
//		ax::Color(0.5, 0.5, 0.5, 0.0), ax::Color(0.0, 0.0, 0.0, 0.0),
//		ax::Color(0.9), 0);
//
//	_close_btn
//		= new ax::Button(this, ax::Rect(rect.size.x - 25 - 5 - 1, 0, 23, 25),
//			ax::Button::Events(GetOnButtonClick()), btn_info, "", "x");
//
//	btn_info.hover = ax::Color(0.0, 0.0, 1.0);
//	btn_info.clicking = ax::Color(0.0, 0.0, 0.95);
//	_min_btn = new ax::Button(this,
//		ax::Rect(rect.size.x - 25 - 5 - 1 - 25, 0, 23, 25),
//		ax::Button::Events(GetOnMinimize()), btn_info, "", "_");
//}

ax::Rect eos::Frame::GetChildRect() const
{
	ax::Rect rect(win->dimension.GetRect());
	int s_w = 4;
	return ax::Rect(s_w + 1, _title_bar_height, rect.size.x - 2 * s_w - 1,
		rect.size.y - _title_bar_height - s_w);
}

void eos::Frame::SetChildHandler(ax::Window::Ptr child)
{
	_child = child;
}

void eos::Frame::SetChildMenuHandler(ax::Window::Ptr child_menu)
{
	_child_menu = child_menu;

	if (_child_menu) {
		_child_menu->Hide();
	}
}

void eos::Frame::OnOpenMenu(const ax::Button::Msg& msg)
{
	if (_menuMode) {
		_menuMode = false;

		if (_child_menu) {
			_child_menu->Hide();
		}

		_child->property.AddProperty("Selectable");
	}
	else {
		_menuMode = true;

		if (_child_menu) {
			_child_menu->Show();
		}

		_child->property.RemoveProperty("Selectable");
	}

	win->Update();
}

void eos::Frame::OnButtonClick(const ax::Button::Msg& msg)
{
	win->PushEvent(Frame::Events::CLOSE, new Frame::Msg(this));
}

void eos::Frame::OnMinimize(const ax::Button::Msg& msg)
{
	win->PushEvent(Frame::Events::MINIMIZE, new Frame::Msg(this));
}

void eos::Frame::OnMouseLeave(const ax::Point& pos)
{
	if (_highlight) {
		_highlight = false;
		//		GetApp()->SetCursor(0);
		//		Update();
	}
}

void eos::Frame::OnMouseMotion(const ax::Point& pos)
{
	ax::Point rel_pos = pos - win->dimension.GetAbsoluteRect().position;

	bool can_resize = win->property.HasProperty("Resizable");

	if (can_resize && rel_pos.x <= 4) {
		//		GetApp()->SetCursor(87);
		_highlight = true;
		win->Update();
		return;
	}

	else if (can_resize && rel_pos.y >= win->dimension.GetSize().y - 4) {
		//		GetApp()->SetCursor(89);
		_highlight = true;
		win->Update();
		return;
	}
	if (_highlight) {
		//		GetApp()->SetCursor(0);
		_highlight = false;
		win->Update();
	}
}

void eos::Frame::OnMouseLeftDown(const ax::Point& pos)
{
	_abs_click_pos = pos;
	ax::Point rel_pos = pos - win->dimension.GetAbsoluteRect().position;

	bool can_resize = win->property.HasProperty("Resizable");

	// Resize from left size.
	if (can_resize && rel_pos.x <= 4) {
		_frame_status = 2;
		_click_pos = rel_pos;
		_click_size = win->dimension.GetSize();
		_highlight = true;
		//		GetApp()->SetCursor(87);
		win->Update();
		win->event.GrabMouse();
	}

	// Move from top frame bar.
	else if (rel_pos.y < 25) {
		_frame_status = 1;
		_click_pos = rel_pos;
		_highlight = true;

		std::cerr << "TODO : Launch desktop event. L 290 eosFrame.cpp"
				  << std::endl;

		std::static_pointer_cast<eos::Desktop>(
			ax::App::GetInstance().GetTopLevel()->backbone)
			->BringToFront(win);
		//        eos::Desktop* desktop =
		//        static_cast<eos::Desktop*>(GetParent());
		//        desktop->BringToFront(this);

		win->Update();
		win->event.GrabMouse();
		//		GetApp()->SetCursor(0);
	}

	// Resize from bottom.
	else if (can_resize && rel_pos.y >= win->dimension.GetSize().y - 4) {
		_frame_status = 3;
		_click_pos = rel_pos;
		_highlight = true;
		_click_size = win->dimension.GetSize();

		//		GetApp()->SetCursor(89);
		win->Update();
		win->event.GrabMouse();
	}

	// In child window.
	else {
		//		GetApp()->SetCursor(0);
	}
}

void eos::Frame::OnMouseLeftUp(const ax::Point& pos)
{
	if (win->event.IsGrabbed()) {
		ax::Point rel_pos = pos - win->dimension.GetAbsoluteRect().position;

		if (rel_pos.x > 4 || rel_pos.y < win->dimension.GetSize().y - 4) {
			_highlight = false;
			//			GetApp()->SetCursor(0);
		}

		if (_frame_status == 2 || _frame_status == 3) {
			if (_child != nullptr) {
				_child->dimension.SetSize(
					win->dimension.GetSize() - ax::Size(8 + 2 * 5, 25 + 4 + 5));
			}
		}

		win->Update();
		win->event.UnGrabMouse();
	}
}

void eos::Frame::OnMouseLeftDragging(const ax::Point& pos)
{

	if (_frame_status == 1) {
		ax::Rect parentRect
			= win->node.GetParent()->dimension.GetAbsoluteRect();
		ax::Point rel_pos = pos - parentRect.position;

		ax::Point prel_pos(rel_pos - _click_pos);
		if (prel_pos.y < 24) {
			prel_pos.y = 24;
		}

		win->dimension.SetPosition(prel_pos);
	}
	else if (_frame_status == 2) {
		ax::Rect parentRect
			= win->node.GetParent()->dimension.GetAbsoluteRect();
		ax::Point rel_pos = pos - parentRect.position;

		// ax::Point local_rel_pos = pos - GetAbsoluteRect().position;
		ax::Point win_pos(win->dimension.GetRect().position);
		win->dimension.SetPosition(
			ax::Point(rel_pos.x - _click_pos.x, win_pos.y));

		ax::Size new_size(_click_size + ax::Size(_abs_click_pos.x - pos.x, 0));

		// ax::Print("New size :", new_size.x, new_size.y);
		win->dimension.SetSize(new_size);
		_close_btn->GetWindow()->dimension.SetPosition(
			ax::Point(win->dimension.GetRect().size.x - 25 - 5 - 1, 0));

		_min_btn->GetWindow()->dimension.SetPosition(
			ax::Point(win->dimension.GetRect().size.x - 25 - 5 - 1 - 25, 0));
		//			if(_child != nullptr)
		//			{
		//				_child->SetSize(new_size - ax::Size(2, 26));
		//			}
	}
	else if (_frame_status == 3) {
		ax::Rect parentRect
			= win->node.GetParent()->dimension.GetAbsoluteRect();
		ax::Point rel_pos = pos - parentRect.position;

		// ax::Point local_rel_pos = pos - GetAbsoluteRect().position;
		// ax::Point win_pos(GetRect().position);
		// SetPosition(ax::Point(win_pos.x, rel_pos.y - _click_pos.y));

		ax::Size new_size(_click_size + ax::Size(0, pos.y - _abs_click_pos.y));

		// ax::Print("New size :", new_size.x, new_size.y);
		win->dimension.SetSize(new_size);
		//_close_btn->SetPosition(ax::Point(GetRect().size.x - 25, 0));

		//			if(_child != nullptr)
		//			{
		//				_child->SetSize(new_size - ax::Size(2, 26));
		//			}
	}
}

void eos::Frame::DrawMenuMode(ax::GC& gc)
{
}

void DrawQuad(const std::vector<ax::Point>& ipoints, ax::Color* colors)
{
	// Order : bl, tl, tr, br.
	ax::Utils::RectPoints<ax::FloatPoint> points
		= { ax::FloatPoint(ipoints[0].x, ipoints[0].y),
			ax::FloatPoint(ipoints[1].x, ipoints[1].y),
			ax::FloatPoint(ipoints[2].x, ipoints[2].y),
			ax::FloatPoint(ipoints[3].x, ipoints[3].y) };

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, &points);
	glColorPointer(4, GL_FLOAT, 0, colors);
	//glDrawArrays(GL_QUADS, 0, 4);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4); 
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
}

void eos::Frame::DrawQuarterCircle(ax::GC gc, const ax::FloatPoint& pos, const int& radius,
	const double& angle, const ax::Color& middle_color,
	const ax::Color& contour_color)
{
	const int& nSegments = 20;

	//glBegin(GL_TRIANGLE_FAN);

	//gc.SetColor(middle_color);
	//glVertex2d(pos.x, pos.y - 1);

	//gc.SetColor(contour_color);
	//for (int i = 0; i <= nSegments; i++) {
		// Get the current angle.
	//	double theta = (2.0f * M_PI) * 0.25 * (double(i)) / double(nSegments);

	//	double x = radius * cosf(theta + angle);
	//	double y = radius * sinf(theta + angle);

	//	glVertex2d(x + pos.x, y + pos.y - 1);
	//}
	//glEnd();
}

void eos::Frame::OnPaintOverFrameBuffer(ax::GC gc)
{
	// if (win->state.Get(ax::Window::StateOption::NeedUpdate)) {
	unsigned char* bg_data = nullptr;
	//	ax::Rect r(-20, -20, win->dimension.GetDrawingRect().size.x + 2 * 20,
	// 20);
	//	//		ax::Rect rect(r.position, r.size);
	//	ax::Rect topRect(
	//		win->dimension.GetDrawingRect().position - ax::Point(20, 20),
	// r.size);

	ax::Rect r(ax::Point(0, -20), win->dimension.GetDrawingRect().size);
	r.size.y = 20;
	win->GetWindowPixelData(bg_data, r);

	//	_img_top_shadow = std::shared_ptr<ax::Image>(
	//		new ax::Image(bg_data, r.size, ax::Image::ColorType::RGBA));
	//	delete bg_data;
	//}

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	gc.SetColor(ax::Color(1.0f, 0.0f, 0.0f, 1.0f));

//	const int top_width = 15;
	const int width = 25;
	ax::Color c0(0.0, 0.01);
	ax::Color c1(0.00, 0.10);
//	ax::Color c1(0.3, 0.25);

	ax::Color t_colors[4] = { c1, c0, c0, c1 };
	ax::Color l_colors[4] = { c0, c0, c1, c1 };
	const ax::Size& ws(win->dimension.GetDrawingRect().size);

	ax::Point in_t_left(0, 0);
//	ax::Point out_t_left(-width, -top_width);
	ax::Point out_t_left(0, -width);
	
	ax::Point in_t_right(ws.x, 0);
//	ax::Point out_t_right(ws.x + width, -top_width);
	ax::Point out_t_right(ws.x, -width);

	ax::Point in_b_left(0, ws.y);
//	ax::Point out_b_left(-width, ws.y + width);
	ax::Point out_b_left(0, ws.y + width);


	ax::Point in_b_right(ws.x, ws.y);
//	ax::Point out_b_right(ws.x + width, ws.y + width);
	ax::Point out_b_right(ws.x, ws.y + width);

	std::vector<ax::Point> top_quad
		= { in_t_left, out_t_left, out_t_right, in_t_right };

	std::vector<ax::Point> left_quad
		= { ax::Point(-width, ws.y), ax::Point(-width, 0), ax::Point(0, 0), ax::Point(0, ws.y) };

	std::vector<ax::Point> right_quad
		= { in_b_right, in_t_right, ax::Point(ws.x + width, 0), ax::Point(ws.x + width, ws.y) };

	std::vector<ax::Point> bottom_quad
		= { ax::Point(0, ws.y + width), ax::Point(0, ws.y), ax::Point(ws.x, ws.y), ax::Point(ws.x, ws.y + width) };

	ax::Color r_colors[4] = { c1, c1, c0, c0 };
	ax::Color b_colors[4] = { c0, c1, c1, c0 };

	//	ax::Utils::RectPoints<ax::FloatPoint> points
	//		= { ax::FloatPoint(top_quad[0].x, top_quad[0].y),
	//			ax::FloatPoint(top_quad[1].x, top_quad[1].y),
	//			ax::FloatPoint(top_quad[2].x, top_quad[2].y),
	//			ax::FloatPoint(top_quad[3].x, top_quad[3].y) };

	DrawQuad(top_quad, t_colors);
	DrawQuad(left_quad, l_colors);
	DrawQuad(right_quad, r_colors);
	DrawQuad(bottom_quad, b_colors);

	// Top left.
	DrawQuarterCircle(gc, ax::FloatPoint(0.0, 1.0), width, M_PI, c1, c0);
	// Top right.
	DrawQuarterCircle(gc, ax::FloatPoint(ws.x, 1.0), width, 3.0 * M_PI * 0.5, c1, c0);
	DrawQuarterCircle(gc, ax::FloatPoint(ws.x, ws.y + 1), width, 0.0, c1, c0);
	DrawQuarterCircle(gc, ax::FloatPoint(0, ws.y + 1), -width, 3.0 * M_PI * 0.5, c1, c0);
	
	// Top right.
//	DrawQuarterCircle(gc, ax::FloatPoint(frect.position.x + frect.size.x - r,
//							  frect.position.y + r - 1),
//		r, 3.0 * M_PI * 0.5, lc1, c1);

	//	{
	//		_shadow_shader.Activate();
	//		GLuint id = _shadow_shader.GetProgramId();
	//		GLint loc = glGetUniformLocation(id, "singleStepOffset");
	//		if (loc != -1) {
	//			glUniform1f(loc, 1.0 / float(300.0));
	//		}
	//
	//		float tex_coords[8] = {
	//			0.0, 1.0, // Top-left.
	//			0.0, 0.0, // Bottom-left.
	//			1.0, 0.0, // Bottom-right.
	//			1.0, 1.0 // Top-right.
	//		};
	//
	//		ax::Color cs0(0.2, 0.4);
	//		ax::Color cs1(0.5, 0.7);
	//		ax::Color shadow_colors[4] = { cs0, cs0, cs1, cs1 };
	//
	//		//		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//		glEnable(GL_TEXTURE_2D);
	//		glBindTexture(GL_TEXTURE_2D, _img_top_shadow->GetTexture());
	//		// win->dimension.GetFrameBuffer()->GetFrameBufferTexture());
	//
	//		glDepthMask(GL_TRUE);
	//
	//		glEnableClientState(GL_VERTEX_ARRAY);
	//		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	//		glEnableClientState(GL_COLOR_ARRAY);
	//		//		glEnableVertexAttribArray(0);
	//
	//		glVertexPointer(2, GL_FLOAT, 0, &points);
	//		glTexCoordPointer(2, GL_FLOAT, 0, tex_coords);
	//		glColorPointer(4, GL_FLOAT, 0, shadow_colors);
	//
	//		glDrawArrays(GL_QUADS, 0, 4);
	//
	//		glDisableClientState(GL_COLOR_ARRAY);
	//		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	//		glDisableClientState(GL_VERTEX_ARRAY);
	//
	//		glDisable(GL_TEXTURE_2D);
	//		glUseProgram(0);
	//	}

	//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//	DrawQuad(top_quad, t_colors);

	//	ax::Rect l_rect(-20, -20,
	//		20, win->dimension.GetDrawingRect().size.y + 2 * 20);
	//
	//	ax::Rect t_rect(0, -20,
	//					win->dimension.GetDrawingRect().size.x, 20);
	//
	//	ax::Rect r_rect(win->dimension.GetDrawingRect().size.x, -20,
	//		20, win->dimension.GetDrawingRect().size.y + 2 * 20);
	//
	//	ax::Rect b_rect(0, win->dimension.GetDrawingRect().size.y,
	//					win->dimension.GetDrawingRect().size.x, 20);
	//
	//	gc.SetColor(ax::Color(0.3, 0.1));
	//	gc.DrawRectangle(l_rect);
	//	gc.DrawRectangle(t_rect);
	//	gc.DrawRectangle(r_rect);
	//	gc.DrawRectangle(b_rect);
}

void eos::Frame::OnPaint(ax::GC gc)
{
	ax::Rect r(win->dimension.GetDrawingRect());

	// Frame rect.
	//	ax::Rect rect(r.position + ax::Point(5, 0), r.size - ax::Size(10, 5));
	ax::Rect rect(r.position, r.size);

	// Title bar.
	ax::Rect topRect(rect.position, ax::Size(rect.size.x, _title_bar_height));

	//if (_bg_img && _bg_img->IsImageReady()) {
	//	_shader.Activate();
	//	GLuint id = _shader.GetProgramId();
	//	GLint loc = glGetUniformLocation(id, "singleStepOffset");
	//	if (loc != -1) {
	//		glUniform1f(loc, 1.0 / float(rect.size.x));
	//	}
	//
	//	gc.DrawImage(_bg_img, rect.position, 0.2);
	//	glUseProgram(0);
	//}

	gc.DrawRectangleColorFade(
		topRect, ax::Color(0.4, 0.4), ax::Color(0.42, 0.42));

	// Frame title.
	gc.SetColor(ax::Color(0.9));
	gc.DrawStringAlignedCenter(_font, _window_name,
		ax::Rect(rect.position, ax::Size(rect.size.x, _title_bar_height)));

	// Frame contour.
	ax::Rect bottomRect(topRect.GetNextPosDown(0),
		ax::Size(rect.size.x, rect.size.y - _title_bar_height));

	gc.DrawRectangleColorFade(
		bottomRect, ax::Color(0.42, 0.42), ax::Color(0.44, 0.44));

	// Frame contour interior.
	gc.SetColor(ax::Color(0.4, 0.4));

	ax::Rect inContour(rect.GetInteriorRect(ax::Point(1, 1)));
	gc.DrawRectangleContour(inContour);

	//    gc.SetColor(ax::Color(0.6));
	gc.SetColor(ax::Color(0.2, 0.4));
	ax::Rect winRect(rect.position.x + 5 - 1,
		rect.position.y + _title_bar_height - 1, rect.size.x - 2 * 5 + 2,
		rect.size.y - _title_bar_height - 5 + 2);

	gc.DrawRectangleContour(winRect);

	// Frame contour.
	gc.SetColor(ax::Color(0.2, 0.4));
	gc.DrawRectangleContour(rect);
}
