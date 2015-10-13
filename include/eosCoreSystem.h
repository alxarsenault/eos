#ifndef __EOS_SYSTEM_H__
#define __EOS_SYSTEM_H__

#include "axLib/axLib.h"
#include "axLib/axDatabase.h"

#include "eosCoreDesktopManager.h"

namespace eos {
	namespace Core {
		
		class User;
		class Tracer;
		
		/*
		 * eos::Core::System
		 */
		class System
		{
		public:
			System();
			
			bool LoginUser(const int& user_id);
			
			std::vector<User> GetUsersList();
			
			const std::shared_ptr<User> GetUser() const;
			
//			Tracer* GetTracer();
			
			std::shared_ptr<DesktopManager> GetDesktopManager();
			
		private:
			std::shared_ptr<DesktopManager> _desktopManager;
//			Manager* _manager;
//			Tracer* _tracer;
			ax::Database* _db;
			std::shared_ptr<User> _current_user;
		};
	}
}

#endif //__EOS_SYSTEM_H__
