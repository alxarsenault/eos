#include "eosTerminal.h"



/*******************************************************************************
 * eos::Terminal.
 ******************************************************************************/
eos::Terminal::Terminal(const ax::Rect& rect, const eos::Terminal::Info& info)
	: _info(info)
	, _font("resources/DejaVuSansMono.ttf")
	, _font_bold("resources/DejaVuSansMono-Bold.ttf")
	, _line_height(15)
	, _start_line_index(0)
{
	win = ax::Window::Create(rect);

	win->event.OnPaint = ax::WBind<ax::GC>(this, &Terminal::OnPaint);

	// Mouse events.
	win->event.OnMouseEnter = ax::WBind<ax::Point>(this, &Terminal::OnMouseEnter);
	win->event.OnMouseLeftDown = ax::WBind<ax::Point>(this, &Terminal::OnMouseLeftDown);
	win->event.OnMouseLeftUp = ax::WBind<ax::Point>(this, &Terminal::OnMouseLeftUp);

	// Keyboard events.
	win->event.OnLeftArrowDown = ax::WBind<char>(this, &Terminal::OnLeftArrowDown);
	win->event.OnRightArrowDown = ax::WBind<char>(this, &Terminal::OnRightArrowDown);
	win->event.OnUpArrowDown = ax::WBind<char>(this, &Terminal::OnUpArrowDown);
	win->event.OnDownArrowDown = ax::WBind<char>(this, &Terminal::OnDownArrowDown);
	win->event.OnKeyDown = ax::WBind<char>(this, &Terminal::OnKeyDown);
	win->event.OnEnterDown = ax::WBind<char>(this, &Terminal::OnEnterDown);
	win->event.OnBackSpaceDown = ax::WBind<char>(this, &Terminal::OnBackSpaceDown);
	win->event.OnKeyDeleteDown = ax::WBind<char>(this, &Terminal::OnKeyDeleteDown);

//	_logic.Init();

	InitTerminal();
}

void eos::Terminal::InitTerminal()
{
	ax::Rect rect(win->dimension.GetRect());

//	_n_line_shown = rect.size.y / _line_height - 1;

	_n_line_shown = 24;

	//	axScrollBarInfo sb_info(ax::Color(0.36), // Normal.
	//							ax::Color(0.38), // Hover.
	//							ax::Color(0.34), // Clicked.
	//							ax::Color(0.3), // Slider contour.
	//							ax::Color(0.36), // Contour.
	//							ax::Color(0.96), // Bg top.
	//							ax::Color(0.90)); // Bg bottom.
	//
	//	axScrollBarEvents sb_evts;
	//	sb_evts.value_change = GetOnScroll();
	//
	//	ax::Rect sb_rect(rect.size.x - 9, 0, 8, rect.size.y - 1);
	//	_scrollBar = new axScrollBar(this, nullptr, sb_rect, sb_evts, sb_info);

	// Create ax::os::Terminal.
	_terminal = ax::shared<ax::os::Terminal>(ax::App::GetInstance().GetEventManager());
	_terminal->AddConnection(ax::os::Terminal::READ_CHILD_BEGIN, GetOnReadChildBegin());
	_terminal->AddConnection(ax::os::Terminal::READ_CHILD_OUTPUT, GetOnReadChildProcess());
	_terminal->Execute("/bin/", "bash", ax::StringVector{"--init-file", "bash_rc.txt"});
}

void eos::Terminal::OnReadChildBegin(const ax::Event::StringMsg& msg)
{
	ax::Print("BEGIN READING");
}

void eos::Terminal::OnReadChildProcess(const ax::Event::StringMsg& msg)
{
//	_logic.AppendOutput(msg.GetMsg());
	_parser.ParseOutput(msg.GetMsg());
	win->Update();
}

// void eos::Terminal::OnScroll(const axScrollBarMsg& msg)
//{
//
//}

void eos::Terminal::OnLeftArrowDown(const char& key)
{
	char cc[3] = {'\033', '[', 'D'};
	_terminal->Write(std::string(cc, 3));
	win->Update();
}

void eos::Terminal::OnRightArrowDown(const char& key)
{
	char cc[3] = {'\033', '[', 'C'};
	_terminal->Write(std::string(cc, 3));
	win->Update();
}

void eos::Terminal::OnUpArrowDown(const char& key)
{
	char cc[3] = {'\033', '[', 'A'};
	_terminal->Write(std::string(cc, 3));
	win->Update();
}

void eos::Terminal::OnDownArrowDown(const char& key)
{
	char cc[3] = {'\033', '[', 'B'};
	_terminal->Write(std::string(cc, 3));
	win->Update();
}

void eos::Terminal::OnKeyDown(const char& key)
{
	_terminal->Write(std::string(1, key));
	win->Update();
}

void eos::Terminal::OnEnterDown(const char& key)
{
	_terminal->Write(std::string(1, (char)13));
	win->Update();
}

void eos::Terminal::OnBackSpaceDown(const char& key)
{
	_terminal->Write(std::string(1, (char)8));
	win->Update();
}

void eos::Terminal::OnKeyDeleteDown(const char& key)
{
	//    _logic.Delete();
	//    int h_size = (int)_logic.GetFileData().size() * _line_height;
	//    _scrollBar->SetPanelSize(ax::Size(GetRect().size.x, h_size));
	//    MoveToCursorPosition();
	//
	//    Update();
}

void eos::Terminal::OnMouseEnter(const ax::Point& mouse)
{
	win->event.GrabKey();
}

void eos::Terminal::OnMouseLeftDown(const ax::Point& mouse_pos)
{
}

void eos::Terminal::OnMouseLeftUp(const ax::Point& mouse_pos)
{
}

void eos::Terminal::OnPaint(ax::GC gc)
{
	ax::Rect rect(win->dimension.GetDrawingRect());

	gc.SetColor(_info.bg_color);
	gc.DrawRectangle(ax::Rect(0, 0, rect.size.x, rect.size.y));

	gc.SetColor(_info.text_color);

	const ax::StringVector& out = _parser.GetOutput();

	const int first_line_pos_y = 5;
	ax::Point line_pos(5, first_line_pos_y);
	ax::Point line_delta(0, _line_height);

	int last_char_pos = line_pos.x;
//	int f_size = _font.GetFontSize();

	int start_i = 0;

//	if (out.size() > _n_line_shown) {
//		start_i = (int)out.size() - _n_line_shown;
//	}

	_is_bold = false;

	// For each lines.
	for (int i = start_i; i < out.size(); i++) {
		const std::string& n = out[i];

		// For each char in line.
		for (int ci = 0, x = line_pos.x; ci < n.size(); ci++) {

			// Look for color change.
			auto it = _parser.GetColorMap().find(CharIndex(ci + 1, i + 1));

			if (it != _parser.GetColorMap().end()) {
				gc.SetColor(it->second);
			}
			
			// Look for bold change.
			auto it_bold = _parser.GetBoldMap().find(CharIndex(ci + 1, i + 1));
			
			if (it_bold != _parser.GetBoldMap().end()) {
				_is_bold = it_bold->second;
			}
			
			ax::Font* font(&_font);
			
			if(_is_bold) {
				font = &_font_bold;
			}
			
			x = gc.DrawChar(*font, n[ci], ax::Point(x, line_pos.y)).x;
			
			
			last_char_pos = x;
			
//			int c_x_test = (int)cursor.x - 1;
//			if(c_x_test < 0) {
//				c_x_test = 0;
//			}
			
//			if(i - start_i == cursor.y && ci == cursor.x) {
////				ax::Print("Found cursor at", i, ci);
//				cursor_position = ax::Point(x, line_pos.y);
//			}
			
		}

		line_pos.y += line_delta.y;
	}

//	std::string cmd = _logic.GetCurrentCommand();
//
//	if (line_pos.y != first_line_pos_y) {
//		line_pos -= line_delta;
//	}
//
//	_next_pos_data.clear();
//	_next_pos_data.resize(cmd.size() + 1);
//	_next_pos_data[0] = last_char_pos;

	// Draw command string.
//	gc.SetColor(ax::Color(0.0));
//
//	for (int i = 0, x = last_char_pos; i < cmd.size(); i++) {
//		_font.SetChar(cmd[i]);
//		ax::Point d = _font.GetDelta();
//
//		ax::Point txtPos(x + d.x, line_pos.y - d.y + f_size);
//
//		ax::Rect txtRect(txtPos, _font.GetSize());
//		gc.DrawTexture(_font.GetTexture(), txtRect);
//
//		x += _font.GetNextPosition();
//
//		_next_pos_data[i + 1] = x;
//
//		last_char_pos = x;
//	}

	// Line cursor.
//	ax::Point cursor_index = _logic.GetCursorIndex();
//	ax::Utils::Point2D<std::size_t> cursor = _logic.GetCursor();

//	ax::Point cursor_pos(_next_pos_data[cursor_index.x], line_pos.y);
//
	ax::Utils::Point2D<std::size_t> cursor = _parser.GetCursor() - ax::Utils::Point2D<std::size_t>(1, 1);
	ax::Point cursor_pos(line_pos.x + (int)cursor.x * 7, first_line_pos_y + (int)cursor.y * 15);
	gc.SetColor(_info.cursor_color);
	gc.DrawRectangle(ax::Rect(cursor_pos, ax::Size(7, 15)));

//	gc.SetColor(_info.bg_color, 0.5);
//	gc.DrawRectangleContour(ax::Rect(1, 0, rect.size.x, rect.size.y));
	//    gc.DrawLine(cursor_pos, cursor_pos + line_delta);
}
