#ifndef __EOS_DESKTOP_H__
#define __EOS_DESKTOP_H__

#include "axLib/axLib.h"
#include "axLib/axButton.h"
#include "axLib/axToggle.h"

#include "eosAppLoader.h"
#include "eosStatusBar.h"
#include "eosFrame.h"
#include "eosDock.h"

#include "eosNotification.h"

#include "eosWindowManager.h"

namespace eos
{
    class Desktop : public axPanel
    {
    public:
        Desktop(eos::WindowManager* eosManager,
                const ax::Rect& rect);
        
        inline eos::WindowManager* GetWindowManager()
        {
            return _eosManager;
        }
        
        inline void AddFrame(eos::Frame* frame)
        {
            _eosManager->AddFrame(frame);
        }
        
        inline void BringToFront(eos::Frame* frame)
        {
            _eosManager->BringToFront(frame);
        }
        
        inline std::vector<eos::Frame*> GetFrameVector()
        {
            return _eosManager->GetFrameVector();
        }
        
        void ShowView();
        
        void ShowNotification();
        
        void ShowTerminal();
        
    private:
        bool _showView;
        bool _terminalMode;
        axWindow* _terminal;
        
        AppLoader* _terminal_app;
        
        eos::WindowManager* _eosManager;
        ax::Image* _bg_img;
        eos::Notification* _notification;
        
        virtual void OnMouseLeftDown(const ax::Point& mouse);
        
        void OnPaint();
        
    };
}

#endif //__EOS_DESKTOP_H__
