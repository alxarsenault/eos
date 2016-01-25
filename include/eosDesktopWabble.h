#ifndef __EOS_DESKTOP_WABBLE_H__
#define __EOS_DESKTOP_WABBLE_H__

#include <axLib/axLib.h>
#include <axLib/axTimer.h>

namespace eos {
	class DesktopWabble;
	
	/*
	 * eos::Dock
	 */
	class DesktopWabble : public ax::Window::Backbone
	{
	public:
		DesktopWabble(const ax::Rect& rect);
		
		
	private:
		ax::Rect _drop_rect, _up_rect;
		
		
		void OnPaint(ax::GC gc);
	};
}

#endif //__EOS_DESKTOP_WABBLE_H__
