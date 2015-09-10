#ifndef __EOS_APP_MANAGER_H__
#define __EOS_APP_MANAGER_H__

#include "axLib/axLib.h"
#include "eosAppLoader.h"
#include "eosFrame.h"

namespace eos
{
    class Desktop;
    
    namespace Core
    {
        /**************************************************************************
         * eos::Core::Manager
         *************************************************************************/
        class Manager
        {
        public:
            Manager();
            
            void SetDesktop(eos::Desktop* desktop)
            {
                _desktop = desktop;
            }
            
            eos::Desktop* GetDesktop()
            {
                return _desktop;
            }
            
            void MainLoop();
            
            inline ax::App* GetMainApp()
            {
                return _axApp;
            }
            
            inline void AddMainEntry(std::function<void()> fct)
            {
                _axApp->AddMainEntry(fct);
            }
            
            inline void AddChild(axWindow* child)
            {
                _childs.push_back(child);
            }
            
            void BringToFront(eos::Frame* frame);
            
            void AddFrame(eos::Frame* frame);
            
            std::vector<eos::Frame*> GetFrameVector();
            
        private:
            eos::Desktop* _desktop;
            ax::App* _axApp;
            
            std::vector<eos::Frame*> _apps;
            std::vector<axWindow*> _childs;
        };
    }
}

#endif //__EOS_APP_MANAGER_H__
