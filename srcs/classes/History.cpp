#include "History.hpp"

History::History(User &user)
{
	_nickname = user.getNickname();
	_username = user.getUsername();
	_hostname = user.getHostname();
	_realname = user.getRealname();
}

History::~History() {};

std::string History::getNickname() const {return _nickname;}
std::string History::getUsername() const {return _username;}
std::string History::getHostname() const {return _hostname;}
std::string History::getRealname() const {return _realname;}