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

#include "eosAlert.h"
#include "eosHome.h"
#include "eosAppViewer.h"

#include "eosCoreSystem.h"

namespace eos
{
    class Desktop : public axPanel
    {
    public:
        
        Desktop(eos::Core::System* system,
                const ax::Rect& rect);
        
        inline eos::Core::Manager* GetWindowManager()
        {
            return _system->GetManager();
        }
        
        inline void AddFrame(eos::Frame* frame)
        {
            _system->GetManager()->AddFrame(frame);
        }
        
        inline void BringToFront(eos::Frame* frame)
        {
            _system->GetManager()->BringToFront(frame);
        }
        
        inline std::vector<eos::Frame*> GetFrameVector()
        {
            return _system->GetManager()->GetFrameVector();
        }
        
        void ShowView();
        
        enum DesktopApps
        {
            DSKT_APP_TERMINAL,
            DSKT_APP_TRACE,
            DSKT_APP_NOTIFY,
            DSKT_APP_HOME,
            DSKT_APP_VIEWER,
            DSKT_APP_COUNT
        };
        
        void ToggleDesktopApp(const DesktopApps& app);
        
    private:
        bool _desktop_apps_status[DSKT_APP_COUNT];
        ax::Window* _desktop_apps[DSKT_APP_COUNT];
        
        bool _showView;
        
        eos::Core::System* _system;
        
        ax::Window* _terminal;
        AppLoader* _terminal_app;
        
        ax::Image* _bg_img;
        eos::Notification* _notification;
        
        eos::Alert* _trace_viewer;
        
        ax::Image* _img_test;
        
        void ReposAllDesktopBuiltInApps();
        
        // Events.
        virtual void OnMouseMotion(const ax::Point& mouse);
        virtual void OnMouseLeftDown(const ax::Point& mouse);
        virtual void OnMouseLeftUp(const ax::Point& mouse);
        
        void OnPaint();
        void PaintView(ax::GC& gc);
        
    };
}

#endif //__EOS_DESKTOP_H__
