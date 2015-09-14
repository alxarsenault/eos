#ifndef __EOS_HOME_H__
#define __EOS_HOME_H__

#include "axLib/axLib.h"
#include "axLib/axButton.h"

namespace eos
{
    /***************************************************************************
     * eos::Home.
     **************************************************************************/
    class Home : public axPanel
    {
    public:
        Home(ax::Window* parent, const ax::Rect& rect);
        
    private:

        void OnPaint();
    };
}

#endif // __EOS_HOME_H__