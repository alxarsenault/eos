#ifndef __EOS_KEYBOARD_H__
#define __EOS_KEYBOARD_H__

#include <axLib/axLib.h>

namespace eos {
	
	class Keyboard : public ax::Window::Backbone {
	public:
		Keyboard(const ax::Rect& rect);
		
	private:
		void OnPaint(ax::GC gc);
	};
}

#endif //__EOS_KEYBOARD_H__
