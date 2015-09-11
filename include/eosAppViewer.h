#ifndef __EOS_APP_VIEWER_H__
#define __EOS_APP_VIEWER_H__

#include "axLib/axLib.h"
#include "axLib/axButton.h"
#include "axLib/axScrollBar.h"

namespace eos
{
    /***************************************************************************
     * eos::Home.
     **************************************************************************/
    class AppViewer : public axPanel
    {
    public:
        AppViewer(axWindow* parent, const ax::Rect& rect);
        
    private:

        void OnPaint();
    };
}

#endif // __EOS_APP_VIEWER_H__