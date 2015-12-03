#ifndef __EOS_APP_VIEWER_H__
#define __EOS_APP_VIEWER_H__

#include "axLib/axLib.h"
#include "axLib/axButton.h"
#include "axLib/axScrollBar.h"
//#include "axLib/axGLShader.h"

namespace eos
{
    /***************************************************************************
     * eos::Home.
     **************************************************************************/
    class AppViewer : public ax::Window::Backbone
    {
    public:
        AppViewer(const ax::Rect& rect);
        
    private:
		//ax::GL::Shader _shader;
		std::shared_ptr<ax::Image> _bg_img;

        void OnPaint(ax::GC gc);
    };
}

#endif // __EOS_APP_VIEWER_H__
