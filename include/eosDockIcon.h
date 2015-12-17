#ifndef __EOS_DOCK_ICON_H__
#define __EOS_DOCK_ICON_H__

#include <axLib/axLib.h>
#include <axLib/axTimer.h>
#include <axLib/axButton.h>

namespace eos {
	/*
	 * eos::DockIcon.
	 */
	class DockIcon : public ax::Window::Backbone
	{
	public:
		DockIcon(const ax::Rect& rect, const std::string& img_path,
			const std::string& name);
		
		void ActivateFullScreenBtn();
		
		std::string GetName() const
		{
			return _name;
		}
		
		inline void SetAlpha(const float& alpha)
		{
			_alpha = alpha;
			win->Update();
		}
		
	private:
		std::shared_ptr<ax::Image> _img;
		ax::Button::Ptr _close_full_screen_btn;
		std::string _name;
		float _alpha;
		
		void OnMouseLeftDown(const ax::Point& mouse);
		void OnMouseEnter(const ax::Point& mouse);
		void OnMouseLeave(const ax::Point& mouse);
		void OnPaint(ax::GC gc);
	};
}

#endif //__EOS_DOCK_ICON_H__
