#include "eosCoreUser.h"

eos::Core::User::User(const int& user_id,
		const std::string& first_name,
		const std::string& last_name,
		const int& profile_type)
	: _user_id(user_id)
	, _first_name(first_name)
	, _last_name(last_name)
	, _profile_type(profile_type)
{}

std::string eos::Core::User::GetFirstName() const
{
	return _first_name;
}

std::string eos::Core::User::GetLastName() const
{
	return _last_name;
}

std::string eos::Core::User::GetFullName() const
{
	if(!_last_name.size()) {
		return _first_name;
	}
	
	return _first_name + " " + _last_name;
}

int eos::Core::User::GetId() const
{
	return _user_id;
}

int eos::Core::User::GetProfileType() const
{
	return _profile_type;
}