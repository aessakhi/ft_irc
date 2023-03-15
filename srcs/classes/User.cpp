#include "User.hpp"

User::User(int fd, std::string hostname): _fd(fd), _user(0),_pwd(0), _nick(0), _authenticated(0),_hostname(hostname), _is_invisible(false), _is_operator(false), _is_wallops(false), _is_away(false)
{
	time(&this->_creatime);
	time(&this->_idletime);
}

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

std::string	User::getUsername() const
{
	return (this->_username);
}

std::string	User::getHostname() const
{
	return (this->_hostname);
}

std::string	User::getRealname() const
{
	return (this->_realname);
}

std::string	User::getOldNick() const
{
	return (this->_oldnick);
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

void	User::setOldnick(std::string oldnick)
{
	this->_oldnick = oldnick;
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

void	User::setAway(bool state, std::string msg)
{
	this->_is_away = state;
	this->_away_msg = msg;
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

const std::string	User::getAwayMessage() const
{
	return this->_away_msg;
}

const std::string	User::getModeString() const
{
	std::string modes("+");

	if (this->isOperator())
		modes += 'o';
	if (this->isInvisible())
		modes += 'i';
	if (this->isWallops())
		modes += 'w';
	if (this->isAway())
		modes += 'a';
	
	if (modes == "+")
		return "";
	
	return modes;
}

int	User::getFd() const
{ return _fd ; }

time_t User::getCreatime() const
{ 
	return (this->_creatime);
}

void	User::updateIdletime()
{
	time(&this->_idletime);
}

time_t	User::getIdletime() const
{
	return (this->_idletime);
}

std::string	&User::getUserbuffer()
{
	return (this->_userbuf);
}
