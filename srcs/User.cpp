#include "User.hpp"

User::User(int fd): _fd(fd), _user(0), _pwd(0), _nick(0), _authenticated(0)
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
