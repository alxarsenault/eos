#ifndef __EOS_SYSTEM_H__
#define __EOS_SYSTEM_H__

#include "axLib/axLib.h"
#include "axLib/axDatabase.h"

#include "eosCoreManager.h"

namespace eos
{
    namespace Core
    {
        class User;
        class Tracer;
        
        class System
        {
        public:
            System();
        
            bool LoginUser(const int& user_id);
            
            std::vector<User> GetUsersList();
        
            const User* GetUser() const;
            
            Tracer* GetTracer();
            
            Manager* GetManager();
            
        private:
            Manager* _manager;
            Tracer* _tracer;
            ax::Database* _db;
            User* _current_user;
        };
    }
}

#endif //__EOS_SYSTEM_H__
