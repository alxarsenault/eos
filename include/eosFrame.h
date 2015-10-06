#ifndef __EOS_FRAME_H__
#define __EOS_FRAME_H__

#include "axLib/axLib.h"
#include "axLib/axButton.h"

namespace eos
{
	class Frame : public axPanel
	{
	public:
		
        class Info
        {
        public:
            Info()
            {
                
            }
        };
        
		/****************************************************************************
		 * eos::Frame::Msg
		 ***************************************************************************/
		class Msg : public ax::Event::Msg
		{
		public:
			Msg();		

			Msg(Frame* sender);

			Frame* GetSender() const;	

			ax::Event::Msg* GetCopy();

		private:
			Frame* _sender;	
				
		};

		class Events
		{
		public:
			enum : ax::Event::Id 
			{
				MINIMIZE,
				CLOSE,
				MOVE,
				RESIZE
			};

			Events(){}
		};		

		Frame(ax::Window* parent,
			  const ax::Rect& rect, 
			  const std::string& window_name);
        
        Frame(ax::App* app,
              const ax::Rect& rect,
              const std::string& window_name);
	
		ax::Rect GetChildRect() const;

		void SetChildHandler(ax::Window* child);
        
        void SetChildMenuHandler(ax::Window* menu);
   
		// Property.
		// Resizable

	private:
        static const int _title_bar_height;
        
        bool _menuMode;
        
//        ax::Button* _info_btn;
        
		ax::Window* _child;
        ax::Window* _child_menu;
        
		std::string _window_name;
		ax::Button *_close_btn, *_min_btn;
        ax::Button* _menu_btn;
		ax::Size _click_size;
		int _frame_status;
		ax::Font _font;
		ax::Point _click_pos, _abs_click_pos;;
		bool _highlight;

        axEVENT_ACCESSOR(ax::Button::Msg, OnOpenMenu);
        void OnOpenMenu(const ax::Button::Msg& msg);

		axEVENT_ACCESSOR(ax::Button::Msg, OnButtonClick);
		void OnButtonClick(const ax::Button::Msg& msg);
	
		axEVENT_ACCESSOR(ax::Button::Msg, OnMinimize);
		void OnMinimize(const ax::Button::Msg& msg);

		void OnMouseLeave();

		void OnMouseMotion(const ax::Point& pos);

		void OnMouseLeftDown(const ax::Point& pos);

		void OnMouseLeftUp(const ax::Point& pos);

		void OnMouseLeftDragging(const ax::Point& pos);

		void OnPaint();
        void DrawMenuMode(ax::GC& gc);
        
        virtual void OnPaintStatic();
	};
}

#endif //__EOS_FRAME_H__
