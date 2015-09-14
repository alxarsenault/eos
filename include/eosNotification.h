#ifndef __EOS_NOTIFICATION_H__
#define __EOS_NOTIFICATION_H__

#include "axLib/axLib.h"
#include "axLib/axTimer.h"
#include "axLib/axButton.h"

namespace eos
{
    /***************************************************************************
     * eos::Notification.
     **************************************************************************/
    class Notification : public axPanel
    {
    public:
        Notification(ax::Window* parent, const ax::Rect& rect);
        
    private:

        void OnPaint();
    };
}

#endif // __EOS_NOTIFICATION_H__