#ifndef __EOS_FRAME_H__
#define __EOS_FRAME_H__

#include "axLib/axLib.h"
#include "axLib/axButton.h"
//#include "axLib/axGLShader.h"

namespace eos
{
	class Frame : public ax::Window::Backbone
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

		Frame(const ax::Rect& rect, 
			  const std::string& window_name);
        
//        Frame(ax::App* app,
//              const ax::Rect& rect,
//              const std::string& window_name);
	
		ax::Rect GetChildRect() const;

		void SetChildHandler(ax::Window::Ptr child);
        
        void SetChildMenuHandler(ax::Window::Ptr menu);
   
		// Property.
		// Resizable

	private:
        static const int _title_bar_height;
        
        bool _menuMode;
		
		ax::Window::Ptr _child;
        ax::Window::Ptr _child_menu;
        
		std::string _window_name;
		ax::Button::Ptr _close_btn, _min_btn;
        ax::Button::Ptr _menu_btn;
		
		ax::Size _click_size;
		int _frame_status;
		ax::Font _font;
		ax::Point _click_pos, _abs_click_pos;;
		bool _highlight;
		
		//ax::GL::Shader _shader;
		//ax::GL::Shader _shadow_shader;

		ax::Image* _bg_img;
//		std::shared_ptr<ax::Image> _img_top_shadow;

        axEVENT_ACCESSOR(ax::Button::Msg, OnOpenMenu);
        void OnOpenMenu(const ax::Button::Msg& msg);

		axEVENT_ACCESSOR(ax::Button::Msg, OnButtonClick);
		void OnButtonClick(const ax::Button::Msg& msg);
	
		axEVENT_ACCESSOR(ax::Button::Msg, OnMinimize);
		void OnMinimize(const ax::Button::Msg& msg);

		void OnMouseLeave(const ax::Point& pos);

		void OnMouseMotion(const ax::Point& pos);

		void OnMouseLeftDown(const ax::Point& pos);

		void OnMouseLeftUp(const ax::Point& pos);

		void OnMouseLeftDragging(const ax::Point& pos);

		void OnPaint(ax::GC gc);
		void OnPaintOverFrameBuffer(ax::GC gc);
        void DrawMenuMode(ax::GC& gc);
		
		void DrawQuarterCircle(ax::GC gc, const ax::FloatPoint& pos, const int& radius,
							   const double& angle, const ax::Color& middle_color,
							   const ax::Color& contour_color);

	};
}

#endif //__EOS_FRAME_H__
