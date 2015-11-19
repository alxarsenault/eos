#ifndef __EOS_HOME_H__
#define __EOS_HOME_H__

#include "axLib/axLib.h"
#include "axLib/axButton.h"
#include "axLib/axGLShader.h"

namespace eos
{
    /***************************************************************************
     * eos::Home.
     **************************************************************************/
    class Home : public ax::Window::Backbone
    {
    public:
        Home(const ax::Rect& rect);
        
    private:
		ax::GL::Shader _shader;
		std::shared_ptr<ax::Image> _bg_img;

        void OnPaint(ax::GC gc);
    };
}

#endif // __EOS_HOME_H__