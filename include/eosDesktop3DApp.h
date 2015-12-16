#ifndef __EOS_DESKTOP_3D_APP_H__
#define __EOS_DESKTOP_3D_APP_H__

#include "axLib/axLib.h"
#include <axLib/axMathVector.h>

namespace eos {
	typedef ax::Math::Vec3<float> Vec3;
	
	class Desktop3DApp : public ax::Window::Backbone {
	public:
		Desktop3DApp(const ax::Rect& rect);
		
	private:
		ax::Image* _img;
		
		void DrawCube();
		
		void OnPaint(ax::GC gc);
		
		void OnPaint3D(ax::GC gc);
	};
}

#endif //__EOS_DESKTOP_3D_APP_H__
