#include "User.hpp"

User::User(int fd): _fd(fd), _user(0), _pwd(0), _nick(0), _authenticated(0), _is_invisible(false), _is_operator(false), _is_wallops(false)
{}

User::~User()
{}

bool	User::getAuth()
{
	return (this->_authenticated);
}

bool	User::getPwd()
{
	return (this->_pwd);
}

bool	User::getNick()
{
	return (this->_nick);
}

bool	User::getUser()
{
	return (this->_user);
}

std::string	User::getNickname() const
{
	return (this->_nickname);
}

std::string	User::getRealname() const
{
	return (this->_realname);
}

/* Set functions for the booleans */

void	User::setAuth(bool state)
{
	this->_authenticated = state;
}

void	User::setPwd(bool state)
{
	this->_pwd = state;
}

void	User::setNick(bool state)
{
	this->_nick = state;
}

void	User::setUser(bool state)
{
	this->_user = state;
}

bool	User::checkAuth()
{
	return (_user && _pwd && _nick);
}

void	User::setNickname(std::string nickname)
{
	this->_nickname = nickname;
}

void	User::setUsername(std::string username)
{
	this->_username = username;
}

void	User::setRealname(std::string realname)
{
	this->_realname = realname;
}

const std::string User::getMask() const
{
	return _nickname + "!" + _username + "@" + _hostname;
}

void	User::setInvisible(bool state)
{
	this->_is_invisible = state;
}

void	User::setOperator(bool state)
{
	this->_is_operator = state;
}

void	User::setWallops(bool state)
{
	this->_is_wallops = state;
}

void	User::setAway(bool state)
{
	this->_is_away = state;
}

bool	User::isInvisible() const
{
	return this->_is_invisible;
}

bool	User::isOperator() const
{
	return this->_is_operator;
}

bool	User::isWallops() const
{
	return this->_is_wallops;
}

bool	User::isAway() const
{
	return this->_is_away;
}
