#ifndef __EOS_DESKTOP_CUBE_H__
#define __EOS_DESKTOP_CUBE_H__

#include "axLib/axLib.h"
#include "axLib/axTimer.h"

namespace eos {
	class DesktopCube : public ax::Window::Backbone
	{
	public:
		DesktopCube(const ax::Rect& rect);
		
		void SetBackgroundImages(const std::vector<ax::Image::Ptr> & images);
		
		int GetFaceSelected() const;
		
	private:
		float _cube_angle;
		int _cube_face_selected;
		
		std::vector<ax::Image::Ptr> _bg_imgs;
	
		void OnPaint3D(ax::GC gc);
		
		void OnLeftArrowDown(const char& c);
		void OnRightArrowDown(const char& c);
	};
}

#endif //__EOS_DESKTOP_CUBE_H__
