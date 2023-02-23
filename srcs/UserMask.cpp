#include "UserMask.hpp"

UserMask::UserMask()
{}

UserMask::UserMask(std::string nickname, std::string username, std::string hostname, std::string realname) : _nickname(nickname), _username(username), _hostname(hostname), _realname(realname)
{}

UserMask::UserMask(User * user) : _nickname(user->getNickname()), _username(user->getUsername()), _hostname(user->getHostname()), _realname(user->getRealname())
{}

UserMask::UserMask(User user) : _nickname(user.getNickname()), _username(user.getUsername()), _hostname(user.getHostname()), _realname(user.getRealname())
{}

UserMask::UserMask(std::string mask)
{
	size_t a = mask.find('!');
	size_t b = mask.find('@');
	_nickname = mask.substr(0, a);
	_username = mask.substr(a + 1, b - (a + 1));
	_hostname = mask.substr(b + 1, mask.npos);
}

UserMask::UserMask(UserMask const & src)
{
	*this = src;
}

UserMask & UserMask::operator=(UserMask const & src)
{
	_nickname = src._nickname;
	_username = src._username;
	_hostname = src._hostname;
	_realname = src._realname;
	return *this;
}

UserMask::~UserMask()
{}

const std::string UserMask::getMask() const
{
	return _nickname + "!" + _username + "@" + _hostname;
}
