#ifndef __EOS_TERMINAL_H__
#define __EOS_TERMINAL_H__

#include "axLib/axLib.h"
#include "axLib/axScrollBar.h"
#include "axLib/axOSTerminal.h"
#include <map>

namespace eos {
	class Terminal : public ax::Window::Backbone
	{
	public:
		
		class Logic
		{
		public:
			Logic();
			
			ax::StringVector& GetOutputData();
			
			const ax::StringVector& GetOutputData() const;
			
			std::string GetCurrentCommand() const;
			
			inline ax::Point GetCursorIndex() const
			{
				return _cursor_pos;
			}
			
			void AppendOutput(const std::string& out_str);
			
			void AddCharToCommand(const char& c);
			
			void Enter();
			
			void BackSpace();
			
			void MoveCursorRight();
			
			void MoveCursorLeft();
			
			void MoveCursorUp();
			
			void MoveCursorDown();
			
			void ClearCommand();
			
			typedef std::pair<unsigned int, unsigned int> CharIndex;
			typedef std::map<CharIndex, ax::Color> ColorMap;
			
			inline ColorMap& GetColorMap()
			{
				return _colors;
			}
			
		private:
			int _last_cmd_index;
			ax::Point _cursor_pos;
			ax::StringVector _past_command;
			ax::StringVector _terminal_output;
			std::string _current_command;
			
			ColorMap _colors;
		};
		
		class Info
		{
		public:
			Info():
			text_color(0.0),
			bg_color(1.0),
			cursor_color(0.4, 0.4)
			{
				
			}
			
			Info(const ax::Color& text_c,
				 const ax::Color& bg_c,
				 const ax::Color& cursor_c):
			text_color(text_c),
			bg_color(bg_c),
			cursor_color(cursor_c)
			{
				
			}
			
			ax::Color text_color;
			ax::Color bg_color;
			ax::Color cursor_color;
		};
		
		Terminal(const ax::Rect& rect, const Info& info);
		
	private:
		Info _info;
		Logic _logic;
		ax::Font _font;
		int _line_height, _start_line_index, _n_line_shown;
		std::vector<int> _next_pos_data;
		
		void InitTerminal();
		
		std::shared_ptr<ax::os::Terminal> _terminal;
//		axScrollBar* _scrollBar;
		
		axEVENT_ACCESSOR(ax::Event::StringMsg, OnReadChildBegin);
		void OnReadChildBegin(const ax::Event::StringMsg& msg);
		
		axEVENT_ACCESSOR(ax::Event::StringMsg, OnReadChildProcess);
		void OnReadChildProcess(const ax::Event::StringMsg& msg);
		
//		axEVENT_ACCESSOR(axScrollBarMsg, OnScroll)
//		void OnScroll(const axScrollBarMsg& msg);
		
		// Keyboard events.
		virtual void OnLeftArrowDown(const char& key);
		
		virtual void OnRightArrowDown(const char& key);
		
		virtual void OnUpArrowDown(const char& key);
		
		virtual void OnDownArrowDown(const char& key);
		
		virtual void OnKeyDown(const char& key);
		
		virtual void OnEnterDown(const char& key);
		
		virtual void OnBackSpaceDown(const char& key);
		
		virtual void OnKeyDeleteDown(const char& key);
		
		// Mouse events.
		virtual void OnMouseEnter(const ax::Point& mouse);
		
		virtual void OnMouseLeftDown(const ax::Point& mouse_pos);
		
		virtual void OnMouseLeftUp(const ax::Point& mouse_pos);
		
		virtual void OnPaint(ax::GC GC);
	};
}

#endif // __EOS_TERMINAL_H__
