#ifndef __EOS_DOCK_ICON_H__
#define __EOS_DOCK_ICON_H__

#include "axLib/axLib.h"
#include "axLib/axTimer.h"

namespace eos {
	/*
	 * eos::DockIcon.
	 */
	class DockIcon : public ax::Window::Backbone
	{
	public:
		DockIcon(const ax::Rect& rect, const std::string& img_path,
			const std::string& name);
				
	private:
		std::shared_ptr<ax::Image> _img;
		std::string _name;
		
		
		void OnMouseLeftDown(const ax::Point& mouse);
		void OnMouseEnter(const ax::Point& mouse);
		void OnMouseLeave(const ax::Point& mouse);
		void OnPaint(ax::GC gc);
	};
}

#endif //__EOS_DOCK_ICON_H__
