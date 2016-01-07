#include "eosTerminalParser.h"

// http://ascii-table.com/ansi-escape-sequences.php

//    char k = '\033';
//    ax::Print("CHARR CVALUE :", (int)k, " END");
//    echo $COLUMNS
//    it's an environement variable
//
//    also you can find out how many lines by
//    echo $LINES

namespace eos {
Parser::Parser()
{
	_cursor_write_pos = ax::Utils::Point2D<std::size_t>(1, 1);
	_terminal_output.push_back(std::string(""));
}

void Parser::ParseOutput(const std::string& str)
{
	if (str.size() == 0) {
		return;
	}

	for (std::size_t i = 0; i < str.size(); i++) {

		const char& c = str[i];

		std::size_t x = _cursor_write_pos.x - 1;
		std::size_t y = _cursor_write_pos.y - 1;
		std::string& line(_terminal_output[y]);
		const std::size_t line_size(line.size());

		switch (c) {
		case '\033':
			i = ParseEscapceSequence(str, i);
			break;

		case 7:
			ax::Print("DEBUG : Bell.");
			break;

		case 8:
			{
				int c_pos_x = (int)_cursor_write_pos.x;

				if (c_pos_x - 1 < 1) {
					_cursor_write_pos.x = 1;
				}
				else {
					_cursor_write_pos.x--;
				}
			}
			break;

		case 9: // Horizontal tab.
			{

				if (_cursor_write_pos.x == 0) {
					//					ax::Print("DEBUG : TAB ->CURSOR X == 0.");
					break;
				}

				if (_cursor_write_pos.y == 0) {
					//					ax::Print("DEBUG : TAB ->CURSOR Y == 0.");
					break;
				}

				if (x == line_size) {
					const std::string tab(AppendAlignedTab(x));
					line.append(tab);
					_cursor_write_pos.x += tab.size();
				}

				// Add tab somewhere in the middle of a line.
				else if (x < line_size) {
					const std::string tab(AppendAlignedTab(x));
					const size_t tab_size(tab.size());

					if (x + tab_size < line_size) {
						line.replace(x, tab_size, tab);
						_cursor_write_pos.x += tab_size;
					}
					// Tab bigger than line width.
					else {
						line.resize(x + tab_size, ' ');
						_cursor_write_pos.x = line_size;
					}
				}
			}
			break;

		// New line.
		case 10:
			if (y == _terminal_size.y - 1) {
				_terminal_output.push_back(std::string(""));
				_terminal_output.erase(_terminal_output.begin());

				//				ax::Print("Scoll donw.");
			}

			else if (y == _terminal_output.size() - 1) {
				_terminal_output.push_back(std::string("")); // Add empty line.
				_cursor_write_pos.y++;
			}

			else if (_cursor_write_pos.y < _terminal_output.size()) {
				_cursor_write_pos.y++;
			}
			else {
				ax::Print("DEBUG : Nothing done with new line.");
			}
			break;

		case 11: // Vertical tab.
			//			ax::Print("DEBUG : Vertical tab.");
			break;

		case 13: // Carriage return.
			_cursor_write_pos.x = 1;
			break;

		default:
			// Append at the end of current line.
			if (x == line_size) {
				line.push_back(c);
				_cursor_write_pos.x++;
			}

			else if (x < line_size) {
				// Replace character.
				line[x] = c;
				_cursor_write_pos.x++;
			}
			else {
				// _cursor_write_pos is bigger thant line width.
				std::size_t str_length(x - line_size);
				line.append(std::string(str_length - 1, ' ') + std::string(1, c));
			}

			break;
		}
	}
}

std::size_t Parser::ParseEscapceSequence(const std::string& out_str, const std::size_t& esc_pos)
{
	// Character at index at esc_pos should be : '\033'.
	std::size_t i = esc_pos;

	// Check if there's something after esc char.
	if (out_str.size() < i + 1) {
		ax::Print("DEBUG : No character after '\033'.");
		return i;
	}

	// Character at index i + 1 should be : '['.
	if (out_str[i + 1] != '[') {
		ax::Print("DEBUG : '[' after escape character.");
		return i;
	}

	ax::StringVector ansi_esc;
	i = GetEscapeSegment(out_str, i + 2, ansi_esc);
	ProcessEscapeSegment(ansi_esc);
	return i;
}

std::size_t Parser::GetEscapeSegment(
	const std::string& out_str, const std::size_t& esc_pos, ax::StringVector& ansi_esc)
{
	// At least one empty string in vector.
	ansi_esc = ax::StringVector(1);

	// At index i-2 is '\033' and at i-1 is '['.
	std::size_t k = esc_pos;

	for (; k < out_str.size(); k++) {
		char c = out_str[k];

		if (isdigit(c)) {
			// Add digit to current line.
			ansi_esc.back().push_back(c);
		}
		else if (c == ';') {
			// Add a new empty string in ansi_esc lines.
			ansi_esc.push_back("");
		}
		else if (c == ' ') {
			// Do nothing.
			continue;
		}
		else if (c == '=') {
			ax::Print("DEBUG : Character '=' is not supported yet.");
			continue;
		}
		else if(c == '?' ) {
			ansi_esc.push_back(std::string(1, c));
			ansi_esc.push_back(std::string(""));
		}
		else {
			ax::Print("OUTPUT : ", c);

			if (ansi_esc[0].empty() && ansi_esc.size() == 1) {
				ansi_esc[0].push_back(c);
			}
			else {
				// This is the last character in esc sequence.
				ansi_esc.push_back(std::string(1, c));
			}

			break;
		}
	}

	return k;
}

void Parser::ProcessEscapeSegment(const ax::StringVector& ansi_esc)
{
	std::string op_code = ansi_esc.back();

	if (op_code == "H") {
		if (ansi_esc.size() == 1) {
			_cursor_write_pos.x = 0;
			_cursor_write_pos.y = 0;
		}
		else if (ansi_esc.size() == 3) {
			//			ax::Print("GOTO POSITION x, y");
			//			ax::Print("SEQ :", ansi_esc[0], ":", ansi_esc[1], ":", ansi_esc[2]);

			int x = std::stoi(ansi_esc[1]);
			int y = std::stoi(ansi_esc[0]);

			if (y > _terminal_output.size() - 1) {
				//				ax::Print("Y POSITION TOO BIG");
				_terminal_output.push_back(std::string(80, ' '));
				_cursor_write_pos.x = x;
				_cursor_write_pos.y = _terminal_output.size() - 1;
				return;
			}

			if (x > _terminal_output[y].size() - 1) {
				// Resize line.
				//				ax::Print("RESIZE LINE");
				int empty_space = x - int(_terminal_output[y].size() - 1);
				_terminal_output[y].append(std::string(empty_space, ' '));
			}

			_cursor_write_pos.x = x;
			_cursor_write_pos.y = y;
		}
		else {
			ax::Print("ERROR POSITION");
		}
	}
	else if (op_code == "f") {
		//		ax::Print("DEBUG : Cursor Position Delta");
	}
	else if (op_code == "A") {
		//		ax::Print("DEBUG : Cursor Up");
	}
	else if (op_code == "B") {
		//		ax::Print("DEBUG : Cursor Down");
	}
	else if (op_code == "C") {
		//		ax::Print("DEBUG : Cursor Foward");
	}
	else if (op_code == "D") {
		//		ax::Print("DEBUG : Cursor Backward");
	}
	else if (op_code == "s") {
		//		ax::Print("DEBUG : Save Cursor Position");
	}
	else if (op_code == "u") {
		//		ax::Print("DEBUG : Restore Cursor Position");
	}
	else if (op_code == "J") {
		//		ax::Print("DEBUG : ONE J          Erase Display");
	}
	else if (op_code == "2J") {
		//		ax::Print("DEBUG : Erase Display");
		ClearScreen();
	}

	

	else if (op_code == "h") {
	
		// Changes the screen width or type to the mode specified by one of the following values:
		// Screen resolution
		// 0	40 x 25 monochrome (text)
		// 1	40 x 25 color (text)
		// 2	80 x 25 monochrome (text)
		// 3	80 x 25 color (text)
		// 4	320 x 200 4-color (graphics)
		// 5	320 x 200 monochrome (graphics)
		// 6	640 x 200 monochrome (graphics)
		// 7	Enables line wrapping
		// 13	320 x 200 color (graphics)
		// 14	640 x 200 color (16-color graphics)
		// 15	640 x 350 monochrome (2-color graphics)
		// 16	640 x 350 color (16-color graphics)
		// 17	640 x 480 monochrome (2-color graphics)
		// 18	640 x 480 color (16-color graphics)
		// 19	320 x 200 color (256-color graphics)
		
		std::cout << "h : ";
		for(auto& n : ansi_esc) {
			std::cout << n << " ";
		}
		std::cout << std::endl;
	}
	else if (op_code == "K") {
		// Clears all characters from the cursor position to the end of the line
		// (including the character at the cursor position).
		if (_cursor_write_pos.y < _terminal_output.size()) {
			std::size_t line_size = _terminal_output[_cursor_write_pos.y - 1].size();
			std::size_t replace_size = line_size - (_cursor_write_pos.x - 1);
			_terminal_output[_cursor_write_pos.y - 1].replace(
				_cursor_write_pos.x - 1, replace_size, std::string(replace_size, ' '));
		}
		else {
			ax::Print("ERASE LINE POSITION Y TOO BIG.");
		}
	}
	else if (op_code == "m") {
		ax::Print("DEBUG : Set Graphics Mode");
		ProcessGraphicMode(ansi_esc);
	}
	else if (op_code == "L") {
		//		ax::Print("DEBUG : New line on top.");
		_terminal_output.insert(_terminal_output.begin(), std::string(""));

		if (_terminal_output.size() > _terminal_size.y) {
			_terminal_output.pop_back();
		}
	}

	else {
		ax::Print("DEBUG : op_code not found :", op_code);
	}
}

void Parser::ProcessGraphicMode(const ax::StringVector& ansi_esc)
{
	CharIndex index(_cursor_write_pos.x, _cursor_write_pos.y);

	for (int att = 0; att < ansi_esc.size() - 1; att++) {
		ax::Print("Att :", ansi_esc[att]);

		try {
			switch (std::stoi(ansi_esc[att])) {
			case 0: // All attributes off.
				_colors[index] = ax::Color(0.0f, 0.0f, 0.0f);
				_bold_map[index] = false;
				break;
			case 1: // Bold on.
				_bold_map[index] = true;
				break;
			case 30: // Black.
				_colors[index] = ax::Color(0.0f, 0.0f, 0.0f);
				break;
			case 31: // Red.
				_colors[index] = ax::Color(1.0f, 0.0f, 0.0f);
				break;
			case 32: // Green.
				_colors[index] = ax::Color(0.0f, 1.0f, 0.0f);
				break;
			case 33: // Yellow.
				_colors[index] = ax::Color(1.0f, 1.0f, 1.0f);
				break;
			case 34: // Blue.
				_colors[index] = ax::Color(0.0f, 0.0f, 1.0f);
				break;
			case 35: // Magenta.
				_colors[index] = ax::Color(1.0f, 0.0f, 1.0f);
				break;
			case 36: // Cyan.
				_colors[index] = ax::Color(0.0f, 1.0f, 1.0f);
				break;
			case 37: // White.
				_colors[index] = ax::Color(0.95);
				break;
			}
		}
		catch (std::invalid_argument e) {
			ax::Print("Fuck up");
		}
	}
}

void Parser::ClearScreen()
{
	_terminal_output.clear();
	for (int i = 0; i < _terminal_size.y; i++) {
		_terminal_output.push_back(std::string(_terminal_size.x, ' '));
	}
	_cursor_write_pos.x = 1;
	_cursor_write_pos.y = 1;
}

std::string Parser::AppendAlignedTab(const std::size_t& tab_position) const
{
	// Align tab space.
	const std::size_t n_spaces = _tab_width - (tab_position % _tab_width);
	return std::string(n_spaces, ' ');
}
}