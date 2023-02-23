#include "UserMask.hpp"

UserMask::UserMask()
{}

UserMask::UserMask(std::string nickname, std::string username, std::string hostname) : _nickname(nickname), _username(username), _hostname(hostname)
{}

UserMask::UserMask(User * user) : _nickname(user->getNickname()), _username(user->getUsername()), _hostname(user->getHostname())
{}

UserMask::UserMask(User user) : _nickname(user.getNickname()), _username(user.getUsername()), _hostname(user.getHostname())
{}

void	UserMask::initFromMask(std::string mask)
{
	size_t a = mask.find('!');
	size_t b = mask.find('@');
	_nickname = mask.substr(0, a);
	_username = mask.substr(a + 1, b - (a + 1));
	_hostname = mask.substr(b + 1, mask.npos);
}

void	UserMask::initFromNick(std::string nick)
{
	_nickname = nick;
	_username = "*";
	_hostname = "*";
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
	return *this;
}

UserMask::~UserMask()
{}

const std::string UserMask::getMask() const
{
	return _nickname + "!" + _username + "@" + _hostname;
}
