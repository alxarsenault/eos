#ifndef __CODE_EDITOR_H__
#define __CODE_EDITOR_H__

#include "axLib/axLib.h"
#include "axLib/axScrollBar.h"
#include "axLib/axTimer.h"

#include <fstream>
#include <set>

class TextEditor : public ax::Window::Backbone {
public:
	class Logic {
	public:
		Logic();

		bool OpenFile(const std::string& file_path);

		bool SaveFile(const std::string& file_path);

		ax::StringVector& GetFileData();

		const ax::StringVector& GetFileData() const;

		std::string GetFilePath() const;

		ax::Point GetCursorPosition() const;

		void MoveCursorRight();

		void MoveCursorLeft();

		void MoveCursorUp();

		void MoveCursorDown();

		void AddChar(const char& c);

		void Enter();

		void Delete();

		void BackSpace();

	private:
		std::string _file_path;
		ax::Point _cursor_pos;
		ax::StringVector _file_data;
	};

	class Info {
	public:
		Info()
			: text_color(0.0)
			, bg_color(1.0)
			, line_number_color(0.3)
			, line_number_bg_color(0.9)
			, cursor_color(0.1)
		{
		}

		Info(const ax::Color& text_c, const ax::Color& bg_c,
			const ax::Color& line_num_c, const ax::Color& line_num_bg_c,
			const ax::Color& cursor_c)
			: text_color(text_c)
			, bg_color(bg_c)
			, line_number_color(line_num_c)
			, line_number_bg_color(line_num_bg_c)
			, cursor_color(cursor_c)
		{
		}

		ax::Color text_color;
		ax::Color bg_color;
		ax::Color line_number_color;
		ax::Color line_number_bg_color;
		ax::Color cursor_color;
	};

	TextEditor(const ax::Rect& rect, const Info& info);

	void Resize(const ax::Size& size);
	
	std::string GetStringContent() const;
	std::string GetFilePath() const;

private:
	ax::Font _font;
	ax::Font _line_num_font;
	Logic _logic;
	Info _info;
	ax::Window::Ptr _scrollPanel;

	std::vector<std::vector<int>> _next_pos_data;

	std::set<std::string> _key_words_cpp;
	std::set<std::string> _number_cpp;

	int _line_height, _file_start_index;
	int _n_line_shown;

//	axScrollBar* _scrollBar;
	ax::ScrollBar::Ptr _scrollBar;

	ax::Point FileCursorPosToNextPosIndex();

	void MoveToCursorPosition();

	axEVENT_ACCESSOR(ax::ScrollBar::Msg, OnScroll);
	void OnScroll(const ax::ScrollBar::Msg& msg);

	// Keyboard events.
	void OnLeftArrowDown(const char& key);

	void OnRightArrowDown(const char& key);

	void OnUpArrowDown(const char& key);

	void OnDownArrowDown(const char& key);

	void OnKeyDown(const char& key);

	void OnEnterDown(const char& key);

	void OnBackSpaceDown(const char& key);

	void OnKeyDeleteDown(const char& key);

	// Mouse events.
	void OnMouseEnter(const ax::Point& mouse);

	void OnMouseLeftDown(const ax::Point& mouse);

	void OnMouseLeftUp(const ax::Point& mouse);

	void OnPaint(ax::GC gc);
};

#endif // __CODE_EDITOR_H__
