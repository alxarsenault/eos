#ifndef __EOS_APP_MANAGER_H__
#define __EOS_APP_MANAGER_H__

#include "axLib/axLib.h"
#include "eosAppLoader.h"
#include "eosFrame.h"

namespace eos
{
	class Desktop;
	
	namespace Core {
		class DesktopManager
		{
		public:
			DesktopManager();
			
			void SetDesktop(std::shared_ptr<Desktop> desktop);
			
			std::shared_ptr<Desktop> GetDesktop();
			
//			inline void AddChild(ax::Window* child)
//			{
//				_childs.push_back(child);
//			}
			
			//void BringToFront(eos::Frame* frame);
			
			//void AddFrame(eos::Frame* frame);
			
			//std::vector<eos::Frame*> GetFrameVector();
			
		private:
			std::shared_ptr<Desktop> _desktop;
			//ax::App* _axApp;
			
			//std::vector<eos::Frame*> _apps;
			//std::vector<ax::Window*> _childs;
		};
	}
}

#endif //__EOS_APP_MANAGER_H__
