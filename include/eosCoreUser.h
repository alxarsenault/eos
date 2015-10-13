#ifndef __EOS_CORE_USER_H__
#define __EOS_CORE_USER_H__

#include "axLib/axLib.h"

namespace eos {
	namespace Core {
		class User
		{
		public:
			User(const int& user_id,
				const std::string& first_name,
				const std::string& last_name,
				const int& profile_type);
			
			std::string GetFirstName() const;
			
			std::string GetLastName() const;
			
			std::string GetFullName() const;
			
			int GetId() const;
			
			int GetProfileType() const;
			
		private:
			int _user_id;
			std::string _first_name, _last_name;
			int _profile_type;
		};
	}
}

#endif //__EOS_CORE_USER_H__
