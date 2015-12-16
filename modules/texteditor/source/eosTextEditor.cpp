#include "eosTextEditor.h"
#include <algorithm>

/*******************************************************************************
 * eos::TextEditor::Logic.
******************************************************************************/
eos::TextEditor::Logic::Logic()
	: _file_path("")
	, _cursor_pos(0, 0)
{
}

bool eos::TextEditor::Logic::OpenFile(const std::string& file_path)
{
	_file_path = file_path;

	std::ifstream t(file_path);

	std::string file_str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

	// Remove all tab for string.
	ax::Utils::String::ReplaceCharWithString(file_str, '\t', "    ");

	_file_data = ax::Utils::String::Split(file_str, "\n");

	_cursor_pos = ax::Point(0, 0);

	return true;
}

bool eos::TextEditor::Logic::SaveFile(const std::string& file_path)
{
	_file_path = file_path;

	std::ofstream out(file_path);

	for (auto& n : _file_data) {
		out << n << std::endl;
	}

	out.close();

	return true;
}

ax::StringVector& eos::TextEditor::Logic::GetFileData()
{
	return _file_data;
}

const ax::StringVector& eos::TextEditor::Logic::GetFileData() const
{
	return _file_data;
}

std::string eos::TextEditor::Logic::GetFilePath() const
{
	return _file_path;
}

ax::Point eos::TextEditor::Logic::GetCursorPosition() const
{
	return _cursor_pos;
}

void eos::TextEditor::Logic::MoveCursorRight()
{
	int x_pos = _cursor_pos.x + 1;

	// Block cursor position at the last char index + 1
	// to allow append at the end of line.
	if (x_pos > _file_data[_cursor_pos.y].size()) {
		x_pos = (int)_file_data[_cursor_pos.y].size();
	}

	_cursor_pos.x = x_pos;
}

void eos::TextEditor::Logic::MoveCursorLeft()
{
	int x_pos = _cursor_pos.x - 1;

	if (x_pos < 0) {
		x_pos = 0;
	}

	_cursor_pos.x = x_pos;
}

void eos::TextEditor::Logic::MoveCursorUp()
{
	int x_pos = _cursor_pos.x;
	int y_pos = _cursor_pos.y - 1;

	// Block cursor at first line.
	if (y_pos < 0) {
		y_pos = 0;
	}

	// Block x cursor position at last char of line + 1.
	if (x_pos > _file_data[y_pos].size()) {
		x_pos = (int)_file_data[y_pos].size();
	}

	_cursor_pos.x = x_pos;
	_cursor_pos.y = y_pos;
}

void eos::TextEditor::Logic::MoveCursorDown()
{
	int x_pos = _cursor_pos.x;
	int y_pos = _cursor_pos.y + 1;

	// Block cursor at last line.
	if (y_pos > (int)_file_data.size() - 1) {
		y_pos = (int)_file_data.size() - 1;

		// ax::Print("Logic :: Cursor last line");

		// Set cursor at the last char of last line.
		x_pos = (int)_file_data[y_pos].size();

		_cursor_pos.x = x_pos;
		_cursor_pos.y = y_pos;
		return;
	}

	// Block x cursor position at last char of line + 1.
	if (x_pos > _file_data[y_pos].size()) {
		x_pos = (int)_file_data[y_pos].size();
	}

	_cursor_pos.x = x_pos;
	_cursor_pos.y = y_pos;
}

void eos::TextEditor::Logic::AddChar(const char& c)
{
	const char TAB = 9;

	// Append at the end of the line.
	if (_cursor_pos.x == _file_data[_cursor_pos.y].size()) {
		//        if(c == TAB)
		//        {
		//            _file_data[_cursor_pos.y].push_back(' ');
		//            _file_data[_cursor_pos.y].push_back(' ');
		//            _file_data[_cursor_pos.y].push_back(' ');
		//            _file_data[_cursor_pos.y].push_back(' ');
		//            _cursor_pos.x += 4;
		//            return;
		//        }

		_file_data[_cursor_pos.y].push_back(c);
		_cursor_pos.x++;
		return;
	}

	// Insert char.
	//    if(c == TAB)
	//    {
	//        _file_data[_cursor_pos.y].insert(_cursor_pos.x, std::string("    "));
	//        _cursor_pos.x += 4;
	//        return;
	//    }

	//	char cc = c;
	_file_data[_cursor_pos.y].insert(_cursor_pos.x, 1, c);
	//_file_data[_cursor_pos.y].insert(_cursor_pos.x, std::string(&cc));
	_cursor_pos.x++;
}

void eos::TextEditor::Logic::Enter()
{
	// Append at the end of the line.
	if (_cursor_pos.x == _file_data[_cursor_pos.y].size()) {
		_file_data.insert(_file_data.begin() + _cursor_pos.y + 1, std::string(""));

		_cursor_pos.x = 0;
		_cursor_pos.y++;
		return;
	}

	// Beginning of not empty line.
	if (_cursor_pos.x == 0) {
		_file_data.insert(_file_data.begin() + _cursor_pos.y, std::string(""));

		_cursor_pos.y++;
		return;
	}

	// Enter in middle of line.
	std::string old_str = _file_data[_cursor_pos.y].substr(0, _cursor_pos.x);
	std::string new_str = _file_data[_cursor_pos.y].substr(_cursor_pos.x);

	_file_data[_cursor_pos.y] = old_str;
	_file_data.insert(_file_data.begin() + _cursor_pos.y + 1, new_str);

	_cursor_pos.x = 0;
	_cursor_pos.y++;
}

void eos::TextEditor::Logic::Delete()
{
	// Nothing to do when delete on last char of last line.
	if (_cursor_pos.x == _file_data[_cursor_pos.y].size() && _cursor_pos.y == _file_data.size() - 1) {
		return;
	}

	// Delete at the end of line.
	if (_cursor_pos.x == _file_data[_cursor_pos.y].size()) {
		std::string next_line = _file_data[_cursor_pos.y + 1];
		_file_data[_cursor_pos.y] += next_line;

		// Delete old line.
		_file_data.erase(_file_data.begin() + _cursor_pos.y + 1);
		return;
	}

	// Remove char in middle of line.
	_file_data[_cursor_pos.y].erase(_file_data[_cursor_pos.y].begin() + _cursor_pos.x);
}

void eos::TextEditor::Logic::BackSpace()
{
	// Nothing to do when backspace at the begenning of first line.
	if (_cursor_pos.x == 0 && _cursor_pos.y == 0) {
		return;
	}

	// Backspace at the beginning of line.
	if (_cursor_pos.x == 0) {
		std::string line = _file_data[_cursor_pos.y];
		int end_line_pos = (int)_file_data[_cursor_pos.y - 1].size();

		// Append line to the line above.
		_file_data[_cursor_pos.y - 1] += line;

		// Delete old line.
		_file_data.erase(_file_data.begin() + _cursor_pos.y);

		_cursor_pos.x = end_line_pos;
		_cursor_pos.y--;
		return;
	}

	// Remove at the end of the line.
	if (_cursor_pos.x == _file_data[_cursor_pos.y].size()) {
		//        ax::Print("POP");
		_file_data[_cursor_pos.y].pop_back();
		_cursor_pos.x--;

		if (_cursor_pos.x < 0) {
			_cursor_pos.x = 0;
		}
		return;
	}

	// Remove char in middle of line.
	_file_data[_cursor_pos.y].erase(_file_data[_cursor_pos.y].begin() + _cursor_pos.x - 1);
	_cursor_pos.x--;
}

/*******************************************************************************
 * eos::TextEditor.
 ******************************************************************************/
eos::TextEditor::TextEditor(const ax::Rect& rect, const eos::TextEditor::Info& info)
	: _font("resources/VeraMono.ttf")
	, _line_num_font("resources/DejaVuSansMono.ttf")
	, _line_height(15)
	, _file_start_index(0)
	, _info(info)
{
	// Create window.
	win = ax::Window::Create(rect);

	win->event.OnPaint = ax::WBind<ax::GC>(this, &TextEditor::OnPaint);

	//    _font.SetFontSize(12);
	_line_num_font.SetFontSize(10);
	_n_line_shown = rect.size.y / _line_height;

	//    axScrollBarInfo scrollInfo(ax::Color(0.6), // Normal.
	//                               ax::Color(0.7), // Hover.
	//                               ax::Color(0.5), // Clicked.
	//                               ax::Color(0.0), // Slider contour.
	//                               ax::Color(0.0), // Contour.
	//                               ax::Color(0.3), // Bg top.
	//                               ax::Color(0.45)); // Bg bottom.

	//    axScrollBarInfo scrollInfo(ax::Color(0.36), // Normal.
	//                               ax::Color(0.38), // Hover.
	//                               ax::Color(0.34), // Clicked.
	//                               ax::Color(0.3), // Slider contour.
	//                               ax::Color(0.96), // Contour.
	//                               ax::Color(0.96), // Bg top.
	//                               ax::Color(0.90)); // Bg bottom.

	//    axScrollBarEvents scrollEvents;
	//    scrollEvents.value_change = GetOnScroll();
	//
	//    _scrollBar = new axScrollBar(this, nullptr,
	//                                 ax::Rect(rect.size.x - 11, 0, 10, rect.size.y - 1),
	//                                 scrollEvents,
	//                                 scrollInfo);

	_logic.OpenFile("resources/text_edit_file_test.cpp");

	int h_size = (int)_logic.GetFileData().size() * _line_height;
	//    _scrollBar->SetPanelSize(ax::Size(rect.size.x, h_size));

	_number_cpp.insert("0");

	_key_words_cpp.insert("class");
	_key_words_cpp.insert("const");
	_key_words_cpp.insert("return");
	_key_words_cpp.insert("void");
	_key_words_cpp.insert("virtual");
	_key_words_cpp.insert("private");
	_key_words_cpp.insert("public");
	_key_words_cpp.insert("protected");
	_key_words_cpp.insert("virtual");
	_key_words_cpp.insert("if");
	_key_words_cpp.insert("else");
	_key_words_cpp.insert("while");
	_key_words_cpp.insert("float");
	_key_words_cpp.insert("double");
	_key_words_cpp.insert("unsigned");
	_key_words_cpp.insert("char");
	_key_words_cpp.insert("for");
	_key_words_cpp.insert("namespace");
	_key_words_cpp.insert("new");
	_key_words_cpp.insert("delete");
}

ax::Point eos::TextEditor::FileCursorPosToNextPosIndex()
{
	ax::Point file_cursor(_logic.GetCursorPosition());

	// Cursor is above shown text.
	if (file_cursor.y < _file_start_index) {
		// ax::Print(ax::Console::Color::Yellow, "Cursor out of screen.");
		return ax::Point(-1, -1);
	}
	// Cursor is bellow shown text.
	if (file_cursor.y > _file_start_index + _n_line_shown - 1) {
		// ax::Print(ax::Console::Color::Yellow, "Cursor out of screen.");
		return ax::Point(-1, -1);
	}

	return ax::Point(file_cursor.x, file_cursor.y - _file_start_index);
}

void eos::TextEditor::MoveToCursorPosition()
{
	ax::Point cur_pos(_logic.GetCursorPosition());

	// Cursor is bellow last shown line.
	if (cur_pos.y > _file_start_index + _n_line_shown - 1) {
		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		// Possible problem when file size is smaller than _n_line_shown.
		//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

		_file_start_index = cur_pos.y - _n_line_shown + 1;
	}
	else if (cur_pos.y < _file_start_index) {
		_file_start_index = cur_pos.y;
	}

	// Move scroll bar.
	//    int diff = (int)_logic.GetFileData().size() - _n_line_shown;
	//    double scroll_ratio = _file_start_index / double(diff);
	//    _scrollBar->SetZeroToOneValue(scroll_ratio);
}

void eos::TextEditor::Resize(const ax::Size& size)
{
	_n_line_shown = size.y / _line_height;

	//    _scrollBar->SetRect(ax::Rect(size.x - 11, 0, 10, size.y - 1));
	//
	//    int h_size = (int)_logic.GetFileData().size() * _line_height;
	//    _scrollBar->SetPanelSize(ax::Size(size.x, h_size));
	//
	//    // Move scroll bar.
	//    int diff = (int)_logic.GetFileData().size() - _n_line_shown;
	//    double scroll_ratio = _file_start_index / double(diff);
	//    _scrollBar->SetZeroToOneValue(scroll_ratio);
	//
	win->dimension.SetSize(size);
}

// void eos::TextEditor::OnScroll(const axScrollBarMsg& msg)
//{
//    int diff = (int)_logic.GetFileData().size() - _n_line_shown;
//
//    double scroll_ratio = msg.GetSender()->GetZeroToOneValue();
//
//    _file_start_index = scroll_ratio * diff;
//
//    win->Update();
//}

void eos::TextEditor::OnMouseEnter()
{
	ax::Print("eos::TextEditor::OnMouseEnter.");
	win->event.GrabKey();
}

void eos::TextEditor::OnLeftArrowDown()
{
	_logic.MoveCursorLeft();
	MoveToCursorPosition();
	win->Update();
}

void eos::TextEditor::OnRightArrowDown()
{
	_logic.MoveCursorRight();
	MoveToCursorPosition();
	win->Update();
}

void eos::TextEditor::OnUpArrowDown()
{
	_logic.MoveCursorUp();
	MoveToCursorPosition();
	win->Update();
}

void eos::TextEditor::OnDownArrowDown()
{
	_logic.MoveCursorDown();
	MoveToCursorPosition();
	win->Update();
}

void eos::TextEditor::OnKeyDown(const char& key)
{
	_logic.AddChar(key);
	MoveToCursorPosition();
	win->Update();
}

void eos::TextEditor::OnEnterDown()
{
	_logic.Enter();

	//	int h_size = (int)_logic.GetFileData().size() * _line_height;
	//	_scrollBar->SetPanelSize(ax::Size(GetRect().size.x, h_size));
	MoveToCursorPosition();

	win->Update();
}

void eos::TextEditor::OnBackSpaceDown()
{
	_logic.BackSpace();
	//	int h_size = (int)_logic.GetFileData().size() * _line_height;
	//	_scrollBar->SetPanelSize(ax::Size(GetRect().size.x, h_size));
	MoveToCursorPosition();

	win->Update();
}

void eos::TextEditor::OnKeyDeleteDown()
{
	_logic.Delete();
	//	int h_size = (int)_logic.GetFileData().size() * _line_height;
	//	_scrollBar->SetPanelSize(ax::Size(GetRect().size.x, h_size));
	MoveToCursorPosition();

	win->Update();
}

void eos::TextEditor::OnMouseLeftDown(const ax::Point& mouse_pos)
{
}

void eos::TextEditor::OnMouseLeftUp(const ax::Point& mouse_pos)
{
	//        ax::Point pos = mouse_pos - GetAbsoluteRect().position;
	//        ax::Size w_size(GetSize());
	//
	//        int n_shown = w_size.y / 15;
	//
	//        _selected_index = (pos.y / (double)w_size.y) * n_shown;
	//
	//        int index = _file_start_index + _selected_index;
	//
	//        const std::vector<ax::os::File>& content = _dir.GetContent();
	//
	//        if(index < content.size())
	//        {
	//            if(content[index].type == ax::os::File::FOLDER)
	//            {
	//                _dir.Goto(_dir.GetPath() + _dir.GetContent()[index].name + "/");
	//                ResetParams();
	//            }
	//        }
	//
	//        Update();
}

bool is_special(const char& x)
{
	return x == '(' || x == ')' || x == ':' || x == '[' || x == ']' || x == ',' || x == ';' || x == '{'
		|| x == '}';
}

// bool is_number_char();

std::string RemoveSpecialChar(const std::string& str)
{
	std::string r = str;
	r.erase(std::remove_if(r.begin(), r.end(), [](char x) { return is_special(x); }), r.end());

	return r;
}

// bool is_number(const std::string& s)
//{
//    return !s.empty() && std::find_if(s.begin(), s.end(),
//                                      [](char c)
//    {
//        return !std::isdigit(c);
//    }) == s.end();
//}

bool is_number(const std::string& str)
{
	char* p;
	strtod(str.c_str(), &p); //, 10);
	return *p == 0;
}

void eos::TextEditor::OnPaint(ax::GC gc)
{
	ax::Rect rect(win->dimension.GetDrawingRect());

	gc.SetColor(_info.bg_color);
	gc.DrawRectangle(ax::Rect(0, 0, rect.size.x, rect.size.y));

	// Draw line number background.
	gc.SetColor(_info.line_number_bg_color);
	gc.DrawRectangle(ax::Rect(0, 0, 25, rect.size.y));

	ax::Point num_pos(4, 2);

	gc.SetColor(_info.line_number_color);

	// Draw line number.
	for (int i = 0; i < _n_line_shown; i++) {
		int num = i + _file_start_index;
		std::string num_str = std::to_string(num);

		if (num < 10) {
			num_str = "  " + num_str;
		}
		else if (num < 100) {
			num_str = " " + num_str;
		}

		gc.DrawString(_line_num_font, num_str, num_pos);
		num_pos += ax::Point(0, 15);
	}

	// Text initial position.
	// ax::Point line_pos(4, 0);
	ax::Point line_pos(25 + 4, 0);

	_next_pos_data.clear();

	const ax::StringVector& data = _logic.GetFileData();

	// Set text color.
	gc.SetColor(_info.text_color);

	gc.shader_font.Activate();
	glEnableClientState(GL_COLOR_ARRAY);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	ax::Color colors[4] = { _info.text_color, _info.text_color,
		_info.text_color, _info.text_color };
	
	glColorPointer(4, GL_FLOAT, 0, (void*)colors);
	

	// Draw text.
	for (int i = 0, k = _file_start_index; k < data.size() && i < _n_line_shown; i++, k++) {
		const std::string& text = data[k];
		std::vector<int> next_vec(text.size() + 1);

		// Draw string.
		if (_font) {
			int x = line_pos.x;

			next_vec[0] = x;
			//
			//----------------------------------------
			//            ax::StringVector words = ax::Utils::String::Split(text, " ");
			//
			//            int index = 0;
			//
			//            for(auto& w : words)
			//            {
			//                std::string clean_word = RemoveSpecialChar(w);
			//
			//                ax::Color word_color = _info.text_color;
			//
			//                if(_key_words_cpp.find(clean_word) != _key_words_cpp.end())
			//                {
			//                    word_color = ax::Color(0.6627451, 0.05098039, 0.5686275);
			//                }
			//
			//                for (int i = 0; i < w.size(); i++)
			//                {
			//                    gc.SetColor(word_color);
			//
			//                    if(text[index] == ' ')
			//                    {
			//                        i--;
			//                        _font.SetChar(' ');
			//                    }
			//                    else
			//                    {
			//                        _font.SetChar(w[i]);
			//
			//                        if(is_special(w[i]))
			//                        {
			//                            gc.SetColor(_info.text_color);
			//                        }
			//                        else if(std::isdigit(w[i]))
			//                        {
			//                            gc.SetColor(ax::Color(0.0, 0.0, 1.0));
			//                        }
			//                    }
			//
			//                    ax::Point d = _font.GetDelta();
			//
			//                    ax::Point txtPos(x + d.x,
			//                                     line_pos.y - d.y + _font.GetFontSize());
			//
			//                    ax::Rect txtRect(txtPos, _font.GetSize());
			//                    gc.DrawTexture(_font.GetTexture(), txtRect);
			//
			//                    x += _font.GetNextPosition();
			//
			//                    next_vec[index + 1] = x;
			//
			//                    index++;
			//
			//                }
			//            }
			//
			//            _font.SetChar(' ');
			//            ax::Point d = _font.GetDelta();
			//
			//            while(index < text.size())
			//            {
			//                ax::Point txtPos(x + d.x,
			//                                 line_pos.y - d.y + _font.GetFontSize());
			//                ax::Rect txtRect(txtPos, _font.GetSize());
			//                gc.DrawTexture(_font.GetTexture(), txtRect);
			//                x += _font.GetNextPosition();
			//                next_vec[index + 1] = x;
			//                index++;
			//            }
			//------------------------

			for (int i = 0; i < text.size(); i++) {
				_font.SetChar(text[i]);
				ax::Point d = _font.GetDelta();

				ax::Point txtPos(x + d.x, line_pos.y - d.y + _font.GetFontSize());

				ax::Rect txtRect(txtPos, _font.GetSize());
				
				
				
				gc.DrawTexture(_font.GetTexture(), txtRect);

				x += _font.GetNextPosition();

				next_vec[i + 1] = x;
			}
		}

		_next_pos_data.push_back(next_vec);
		line_pos += ax::Point(0, 15);
	}
	
	glDisableClientState(GL_COLOR_ARRAY);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,
						GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	gc.shader_fb.Activate();

	// Line cursor.
	ax::Point cursor_index = FileCursorPosToNextPosIndex();

	if (cursor_index.x != -1 && cursor_index.y != -1) {
		ax::Print("Draw cursor");
		int x = _next_pos_data[cursor_index.y][cursor_index.x];
		int y = cursor_index.y * _line_height;

		//        gc.SetColor(_info.cursor_color);
		gc.SetColor(ax::Color(1.0f, 0.0f, 0.0f));
		gc.DrawLine(ax::Point(x, y), ax::Point(x, y + _line_height));
	}
}
