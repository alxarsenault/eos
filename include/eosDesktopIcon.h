#ifndef __EOS_DESKTOP_ICON_H__
#define __EOS_DESKTOP_ICON_H__

#include "axLib/axLib.h"

namespace eos {

	class Desktop;
	
	/*
	 * eos::Core::DesktopIcon.
	 */
	class DesktopIcon : public ax::Window::Backbone
	{
	public:
		DesktopIcon(Desktop* desktop, const ax::Point& pos,
				const std::string& img_path, const std::string& name);
		
		ax::Point GetGridIndex() const;
		
		void SetGridIndex(const ax::Point& index);
		
	private:
		Desktop* _desktop;
		ax::Point _clickPos;
		ax::Color _current_color;
		std::shared_ptr<ax::Image> _img;
		ax::Point _grid_index;
		std::string _name;
		ax::Font _font;
		
		void OnMouseEnter(const ax::Point& mouse);
		
		void OnMouseLeave(const ax::Point& mouse);
		
		void OnMouseMotion(const ax::Point& mouse);
		
		void OnMouseLeftDown(const ax::Point& mouse);
		
		void OnMouseLeftUp(const ax::Point& mouse);
		
		void OnMouseLeftDragging(const ax::Point& mouse);
		
		void OnPaint(ax::GC gc);
	};
}

#endif //__EOS_DESKTOP_H__
