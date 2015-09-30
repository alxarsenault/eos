#ifndef __EOS_TERMINAL_H__
#define __EOS_TERMINAL_H__

#include "axLib/axLib.h"
#include "axLib/axScrollBar.h"
#include "axOSTerminal.h"

namespace eos
{
    class Terminal : public axPanel
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
            
        private:
            int _last_cmd_index;
            ax::Point _cursor_pos;
            ax::StringVector _past_command;
            ax::StringVector _terminal_output;
            std::string _current_command;
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
        
        Terminal(ax::Window* parent, const ax::Rect& rect, const Info& info);
        
    private:
        Info _info;
        Logic _logic;
        ax::Font _font;
        int _line_height, _start_line_index, _n_line_shown;
        std::vector<int> _next_pos_data;
        
        ax::os::Terminal* _terminal;
        axScrollBar* _scrollBar;
        
        axEVENT_ACCESSOR(ax::Event::StringMsg, OnReadChildBegin);
        void OnReadChildBegin(const ax::Event::StringMsg& msg);
        
        axEVENT_ACCESSOR(ax::Event::StringMsg, OnReadChildProcess);
        void OnReadChildProcess(const ax::Event::StringMsg& msg);
        
        axEVENT_ACCESSOR(axScrollBarMsg, OnScroll)
        void OnScroll(const axScrollBarMsg& msg);
        
        // Keyboard events.
        virtual void OnLeftArrowDown();
        
        virtual void OnRightArrowDown();
        
        virtual void OnUpArrowDown();
        
        virtual void OnDownArrowDown();
        
        virtual void OnKeyDown(const char& key);
        
        virtual void OnEnterDown();
        
        virtual void OnBackSpaceDown();
        
        virtual void OnKeyDeleteDown();
        
        // Mouse events.
        virtual void OnMouseEnter();
        
        virtual void OnMouseLeftDown(const ax::Point& mouse_pos);
        
        virtual void OnMouseLeftUp(const ax::Point& mouse_pos);
        
        
        virtual void OnPaint();

    };
}

#endif // __EOS_TERMINAL_H__
