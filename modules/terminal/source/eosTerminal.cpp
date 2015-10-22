#include "eosTerminal.h"

//http://ascii-table.com/ansi-escape-sequences.php

/*******************************************************************************
 * eos::Terminal::Logic.
 ******************************************************************************/
eos::Terminal::Logic::Logic()
	: _cursor_pos(0, 0)
	, _current_command("")
	, _last_cmd_index(0)
{}

ax::StringVector& eos::Terminal::Logic::GetOutputData()
{
	return _terminal_output;
}

const ax::StringVector& eos::Terminal::Logic::GetOutputData() const
{
	return _terminal_output;
}

std::string eos::Terminal::Logic::GetCurrentCommand() const
{
	return _current_command;
}

void eos::Terminal::Logic::AppendOutput(const std::string& out_str)
{
	std::string str = out_str;
	
	if(str.size() == 0) {
		return;
	}
	
	for(int i = 0; i < str.size(); i++) {
		if(str[i] == '\n') {
			// Add new line.
			_terminal_output.push_back("");
		} else if(str[i] == '\t') {
			// Align tab space.
			switch(_terminal_output.back().size() % 8) {
				case 0 : _terminal_output.back().append("        "); break;
				case 1 : _terminal_output.back().append("       "); break;
				case 2 : _terminal_output.back().append("      "); break;
				case 3 : _terminal_output.back().append("     "); break;
				case 4 : _terminal_output.back().append("    "); break;
				case 5 : _terminal_output.back().append("   "); break;
				case 6 : _terminal_output.back().append("  "); break;
				case 7 : _terminal_output.back().append(" "); break;
			}
		}
		
		// Vertical tab.
		else if(str[i] == 13) {
			//                ax::Print("Vertical tab.");
		}
		else if(str[i] == '\033') {
			//            ax::Print("ESC");
			if(i + 1 < str.size()) {
				if(str[i+1] == '[') {
					bool go = true;
					int k = 2;
					
					ax::StringVector ansi_esc(1);
					
					while(go) {
						if(k < str.size()) {
							char n = str[i+k];
							
							if(isdigit(n)) {
								ansi_esc.back().push_back(n);
							} else if(n == ';') {
								ansi_esc.push_back("");
							} else if(n == ' ') {
								continue;
							} else if(n == '=') {
								ax::Print("=====");
								continue;
							} else {
								ansi_esc.push_back("");
								ansi_esc.back().push_back(n);
								go = false;
							}
							k++;
						}
					}
					
					std::string op_code = ansi_esc.back();
					
					if(op_code == "H") {
						ax::Print("Cursor Position");
					} else if(op_code == "f") {
						ax::Print("Cursor Position Delta");
					} else if(op_code == "A") {
						ax::Print("Cursor Up");
					} else if(op_code == "B") {
						ax::Print("Cursor Down");
					} else if(op_code == "C") {
						ax::Print("Cursor Foward");
					} else if(op_code == "D") {
						ax::Print("Cursor Backward");
					} else if(op_code == "s") {
						ax::Print("Save Cursor Position");
					} else if(op_code == "u") {
						ax::Print("Restore Cursor Position");
					} else if(op_code == "2J") {
						ax::Print(" Erase Display");
					} else if(op_code == "k") {
						ax::Print("Erase Line");
					} else if(op_code == "m") {
						ax::Print("Set Graphics Mode");
						
						unsigned int char_index = 0;
						
						if(_terminal_output.size() &&
								_terminal_output.back().size()) {
								char_index = (unsigned int)_terminal_output.back().size();
						}
						
						i += k - 1;
						
						unsigned int line_index = (unsigned int)_terminal_output.size() - 1;
						
						if(str[i+1] == '\n' || str[i+1] == 13) {
							ax::Print("Enter after", str[i+1], int(str[i+1]));
							line_index += 1;
							char_index = 0;
						}
						
						CharIndex index(line_index, char_index);
						
						//ax::Print("INDEX :", index.first, index.second);
						
//						ax::Print("eSC");

//						for(auto& n : ansi_esc) {
//							for(auto& c : n) {
//								ax::Print(n, " :", c, (int)c);
//							}
//						}
						for(int att = 0; att < ansi_esc.size() - 1; att++) {
		
							try {
								ax::Print("Att :", ansi_esc[att], (int)ansi_esc[att][0]);
								switch(std::stoi(ansi_esc[att])) {
									case 30: _colors[index] = ax::Color(0.0f, 0.0f, 0.0f); break;
									case 31: _colors[index] = ax::Color(1.0f, 0.0f, 0.0f); break;
									case 32: _colors[index] = ax::Color(0.0f, 1.0f, 0.0f); break;
									}
							} catch(std::invalid_argument e) {
								ax::Print("Fuck up");
							
							}
						}
						
						
					}
				}
			}
		} else {
			if(_terminal_output.size()) {
				_terminal_output.back().push_back(str[i]);
			}
			
			// First char of output.
			else {
				_terminal_output.push_back(" ");
				_terminal_output[0] = str[i];
			}
		}
	}
}

void eos::Terminal::Logic::AddCharToCommand(const char& c)
{
	// Append at the end of the line.
	if(_cursor_pos.x == _current_command.size()) {
		_current_command.push_back(c);
		_cursor_pos.x++;
		return;
	}
	
	// Insert char.
	_current_command.insert(_cursor_pos.x, 1, c);
	_cursor_pos.x++;
}

void eos::Terminal::Logic::Enter()
{
	if(_current_command.size()) {
		_past_command.push_back(_current_command);
	}
	
	_current_command.push_back('\n');
}

void eos::Terminal::Logic::BackSpace()
{
	// Nothing to do when backspace at the begenning command.
	if(_cursor_pos.x == 0) {
		return;
	}
	
	// Remove at the end of the line.
	if(_cursor_pos.x == _current_command.size()) {
		_current_command.pop_back();
		_cursor_pos.x--;
		
		if(_cursor_pos.x < 0) {
			_cursor_pos.x = 0;
		}
		
		return;
	}
	
	// Remove char in middle of line.
	_current_command.erase(_current_command.begin() + _cursor_pos.x - 1);
	_cursor_pos.x--;
}

void eos::Terminal::Logic::MoveCursorRight()
{
	_cursor_pos.x++;
	
	if(_cursor_pos.x > (int)_current_command.size()) {
		_cursor_pos.x = (int)_current_command.size();
	}
}

void eos::Terminal::Logic::MoveCursorLeft()
{
	_cursor_pos.x--;
	
	if(_cursor_pos.x < 0) {
		_cursor_pos.x = 0;
	}
}

void eos::Terminal::Logic::MoveCursorUp()
{
	if(_past_command.size()) {
		if(_last_cmd_index < _past_command.size()) {
			int cmd_index = (int)_past_command.size() - _last_cmd_index - 1;
			std::string cmd = _past_command[cmd_index];
			
			_current_command = cmd;
			_cursor_pos.x = (int)_current_command.size();
		}
		
		_last_cmd_index++;
		
		if(_last_cmd_index == (int)_past_command.size()) {
			_last_cmd_index = (int)_past_command.size() - 1;
		}
	}
}

void eos::Terminal::Logic::MoveCursorDown()
{
	if(_past_command.size()) {
		_last_cmd_index--;
		
		if(_last_cmd_index < 0) {
			_last_cmd_index = 0;
		}
		
		if(_last_cmd_index < _past_command.size()) {
			int cmd_index = (int)_past_command.size() - _last_cmd_index - 1;
			std::string cmd = _past_command[cmd_index];
			
			_current_command = cmd;
			_cursor_pos.x = (int)_current_command.size();
		}
	}
}

void eos::Terminal::Logic::ClearCommand()
{
	//    _past_command.push_back(_current_command);
	
	_current_command.clear();
	_cursor_pos = ax::Point(0, 0);
	_last_cmd_index = 0;
}


/*******************************************************************************
 * eos::Terminal.
 ******************************************************************************/
eos::Terminal::Terminal(const ax::Rect& rect,
						const eos::Terminal::Info& info):
_info(info),
_font("resource/DejaVuSansMono.ttf"),
_line_height(15),
_start_line_index(0)
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
	
	//    char k = '\033';
	//    ax::Print("CHARR CVALUE :", (int)k, " END");
	//    echo $COLUMNS
	//    it's an environement variable
	//
	//    also you can find out how many lines by
	//    echo $LINES
	
	
	InitTerminal();
}

void eos::Terminal::InitTerminal()
{
	ax::Rect rect(win->dimension.GetRect());
	
	_n_line_shown = rect.size.y / _line_height - 1;
	
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
	_terminal = std::shared_ptr<ax::os::Terminal>(
			new ax::os::Terminal(ax::App::GetInstance().GetEventManager()));
	
	_terminal->AddConnection(ax::os::Terminal::READ_CHILD_BEGIN,
							 GetOnReadChildBegin());
	
	// Connect terminal reading event.
	_terminal->AddConnection(ax::os::Terminal::READ_CHILD_OUTPUT,
							 GetOnReadChildProcess());
	
	_terminal->Execute("/bin/", "bash", ax::StringVector());
}

void eos::Terminal::OnReadChildBegin(const ax::Event::StringMsg& msg)
{
	ax::Print("BEGIN READING");
}

void eos::Terminal::OnReadChildProcess(const ax::Event::StringMsg& msg)
{
	_logic.AppendOutput(msg.GetMsg());
	win->Update();
}

//void eos::Terminal::OnScroll(const axScrollBarMsg& msg)
//{
//	
//}

void eos::Terminal::OnLeftArrowDown(const char& key)
{
	_logic.MoveCursorLeft();
	win->Update();
}

void eos::Terminal::OnRightArrowDown(const char& key)
{
	_logic.MoveCursorRight();
	win->Update();
}

void eos::Terminal::OnUpArrowDown(const char& key)
{
	_logic.MoveCursorUp();
	win->Update();
}

void eos::Terminal::OnDownArrowDown(const char& key)
{
	_logic.MoveCursorDown();
	win->Update();
}

void eos::Terminal::OnKeyDown(const char& key)
{
	_logic.AddCharToCommand(key);
	win->Update();
}

void eos::Terminal::OnEnterDown(const char& key)
{
	_logic.Enter();
	std::string cmd = _logic.GetCurrentCommand();
	_logic.ClearCommand();
	
	_terminal->Write(cmd);
	
	win->Update();
}

void eos::Terminal::OnBackSpaceDown(const char& key)
{
	_logic.BackSpace();
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
	
	const ax::StringVector& out = _logic.GetOutputData();
	
	const int first_line_pos_y = 5;
	ax::Point line_pos(5, first_line_pos_y);
	ax::Point line_delta(0, _line_height);
	
	int last_char_pos = line_pos.x;
	int f_size = _font.GetFontSize();
	
	int start_i = 0;
	
	if(out.size() > _n_line_shown) {
		start_i = (int)out.size() - _n_line_shown;
	}
	
	// For each lines.
	for(int i = start_i; i < out.size(); i++) {
		const std::string& n = out[i];
		
		// For each char in line.
		for (int ci = 0, x = line_pos.x; ci < n.size(); ci++) {
			
			// Look for color change.
			auto it = _logic.GetColorMap().find(Logic::CharIndex(i, ci));
			
			if(it != _logic.GetColorMap().end()) {
				gc.SetColor(it->second);
			}
			
			_font.SetChar(n[ci]);
			ax::Point d = _font.GetDelta();
			
			ax::Point txtPos(x + d.x, line_pos.y - d.y + f_size);
			
			ax::Rect txtRect(txtPos, _font.GetSize());
			gc.DrawTexture(_font.GetTexture(), txtRect);
			
			x += _font.GetNextPosition();
			
			last_char_pos = x;
		}
		
		line_pos += line_delta;
	}
	
	
	std::string cmd = _logic.GetCurrentCommand();
	
	if(line_pos.y != first_line_pos_y)
	{
		line_pos -= line_delta;
	}
	
	_next_pos_data.clear();
	_next_pos_data.resize(cmd.size() + 1);
	_next_pos_data[0] = last_char_pos;
	
	// Draw command string.
	//gc.SetColor(ax::Color(0.0, 0.0, 0.0));
	
	for (int i = 0, x = last_char_pos; i < cmd.size(); i++)
	{
		_font.SetChar(cmd[i]);
		ax::Point d = _font.GetDelta();
		
		ax::Point txtPos(x + d.x, line_pos.y - d.y + f_size);
		
		ax::Rect txtRect(txtPos, _font.GetSize());
		gc.DrawTexture(_font.GetTexture(), txtRect);
		
		x += _font.GetNextPosition();
		
		_next_pos_data[i + 1] = x;
		
		last_char_pos = x;
	}
	
	// Line cursor.
	ax::Point cursor_index = _logic.GetCursorIndex();
	
	ax::Point cursor_pos(_next_pos_data[cursor_index.x], line_pos.y);
	
	gc.SetColor(_info.cursor_color);
	gc.DrawRectangle(ax::Rect(cursor_pos, ax::Size(7, 15)));
	
	gc.SetColor(_info.bg_color, 0.5);
	gc.DrawRectangleContour(ax::Rect(1, 0, rect.size.x, rect.size.y));
	//    gc.DrawLine(cursor_pos, cursor_pos + line_delta);
}
