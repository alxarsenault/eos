#include "eosCoreSystem.h"
#include "eosCoreUser.h"


eos::Core::System::System()
//_current_user(nullptr)
{
	_desktopManager = std::shared_ptr<eos::Core::DesktopManager>(
			new eos::Core::DesktopManager());
	
	_tracer = std::unique_ptr<Tracer>(new eos::Core::Tracer());
	
	// Open system database.
	//_db = std::unique_ptr<ax::Database>(new ax::Database("resources/system"));
	
	_tracer->Write("Open system database.");
}

bool eos::Core::System::LoginUser(const int& user_id)
{
	//std::string request = "SELECT * FROM USERS WHERE id = " +
	//	std::to_string(user_id) + ";";
	//
	//std::vector<ax::StringPairVector> users = _db->Request(request);
	//
	//if(!users.size()) {
	//	ax::Error("Can't login, users id", user_id, "doesn't exist.");
	//	_tracer->Write("Can't login, users id", user_id, "doesn't exist.");
	//	return false;
	//}
	//
	//if(users.size() > 1)
	//{
	//	ax::Error("Multiple users with the same id.");
	//	_tracer->Write("Multiple users with the same id.");
	//	return false;
	//}
	//
	//if(_current_user) {
	//	ax::Error("User", _current_user->GetFullName() ,"already login.");
	//	_tracer->Write("User", _current_user->GetFullName() ,"already login.");
	//	
	//	return false;
	//}
	
	//const ax::StringPairVector& usr = users[0];
	//
	//_current_user = std::shared_ptr<User>(
	//	new eos::Core::User(std::stoi(usr[0].second), usr[1].second,
	//		usr[2].second, std::stoi(usr[3].second)));
	//
	//_tracer->Write("Login user :", _current_user->GetFullName());
	
	return true;
}

std::vector<eos::Core::User> eos::Core::System::GetUsersList()
{
	return std::vector<eos::Core::User>();
	//std::vector<eos::Core::User> users_list;
	//std::vector<ax::StringPairVector> users =
	//	_db->Request("SELECT * FROM USERS;");
	
	//if(!users.size()) {
	//	return users_list;
	//}
	
	//users_list.reserve(users.size());
	
	//for(auto& n : users)
	//{
		// TABLE : USERS.
		// ID, FIRSTNAME, LASTNAME, PROFILE_TYPE.
	//	users_list.emplace_back(eos::Core::User(std::stoi(n[0].second),
	//		n[1].second, n[2].second, std::stoi(n[3].second)));
	//}
	
	//return users_list;
}

const std::shared_ptr<eos::Core::User> eos::Core::System::GetUser() const
{
	return _current_user;
}

eos::Core::Tracer* eos::Core::System::GetTracer()
{
	return _tracer.get();
}

std::shared_ptr<eos::Core::DesktopManager>
	eos::Core::System::GetDesktopManager()
{
	return _desktopManager;
}
