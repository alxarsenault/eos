#ifndef __EOS_SYSTEM_H__
#define __EOS_SYSTEM_H__

#include "axLib/axLib.h"
//#include "axLib/axDatabase.h"
#include "eosCoreTracer.h"
#include "eosCoreDesktopManager.h"

namespace eos {
	namespace Core {
		
		class User;
		
		/*
		 * eos::Core::System
		 */
		class System
		{
		public:
			System();
			
			bool LoginUser(const int& user_id);
			
			/// Get list of possible user login.
			std::vector<User> GetUsersList();
			
			const std::shared_ptr<User> GetUser() const;
			
			Tracer* GetTracer();
			
			std::shared_ptr<DesktopManager> GetDesktopManager();
			
		private:
			std::shared_ptr<DesktopManager> _desktopManager;
//			Manager* _manager;
			std::unique_ptr<Tracer> _tracer;
			//std::unique_ptr<ax::Database> _db;
			std::shared_ptr<User> _current_user;
		};
	}
}

#endif //__EOS_SYSTEM_H__
