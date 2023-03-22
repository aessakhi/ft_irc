#include "Channel.hpp"

Channel::Channel(const std::string & name) : _name(name), _modestring("n"), _limit_mode(false), _invite_mode(false), _key_mode(false), _moderated_mode(false), _secret_mode(false), _protected_topic_mode(false), _no_external_messages_mode(true) 
{
	time(&this->_creatime);
	time(&this->_topictime);
}

Channel::~Channel() {}

/* ----------ACCESSORS---------- */

const std::string & Channel::getName() const
{ return _name ; }

std::string Channel::getModes() const
{
	std::string ret;
	if (_modestring.size())
		ret += '+';
	ret += _modestring;
	if (_limit_mode)
	{
		std::stringstream out;
		out << ' ' << _capacity;
		ret += out.str();
	}
	return ret ;
}

const std::string & Channel::getTopic() const
{ return _topic ; }

bool	Channel::limitMode() const
{ return _limit_mode ; }

bool	Channel::inviteMode() const
{ return _invite_mode ; }

bool	Channel::keyMode() const
{ return _key_mode ; }

bool	Channel::moderatedMode() const
{ return _moderated_mode ; }

bool	Channel::secretMode() const
{ return _secret_mode ; }

bool	Channel::protectedTopicMode() const
{ return _protected_topic_mode ; }

bool	Channel::noExternalMessagesMode() const
{ return _no_external_messages_mode ; }

size_t	Channel::capacity() const
{ return _capacity ; }

std::vector<User *>		Channel::getUsers() const
{ return _members ; }

std::vector<std::string>	Channel::getOperators() const
{ return _operators ; }

std::vector<std::string>	Channel::getBanned() const
{ return _banned ; }

std::vector<std::string>	Channel::getBanExcept() const
{ return _ban_except ; }

std::vector<std::string>	Channel::getInviteExcept() const
{ return _invite_except ; }

std::vector<std::string>	Channel::getVoiced() const
{ return _voiced ; }

time_t Channel::getCreatime() const
{ return _creatime; }

time_t Channel::getTopictime() const
{ return _topictime; }

/* ----------ADDING USERS TO LISTS---------- */

void	Channel::addMember(User *user) /* Need to add a check if a user is already in the list */
{ _members.push_back(user); }

void	Channel::addOperator(std::string user)
{ _operators.push_back(user); }

void	Channel::banUser(std::string user)
{ _banned.push_back(user); }

void	Channel::addBanExcept(std::string user)
{ _ban_except.push_back(user); }

void	Channel::addInviteExcept(std::string user)
{ _invite_except.push_back(user); }

void	Channel::addInvite(User * user)
{ _invited.push_back(user) ; }

void	Channel::addVoiced(std::string user)
{ _voiced.push_back(user) ; }

/* ----------REMOVING USERMASKS FROM LISTS---------- */

void	Channel::removeOperator(std::string str)
{
	std::vector<std::string>::iterator it = _operators.begin();
	std::vector<std::string>::iterator ite = _operators.end();
	std::vector<std::string>::iterator it_copy;
	for (; it != ite; it++)
	{
		if (sided_wildcompare(str, *it))
		{
			it_copy = it;
			it--;
			_operators.erase(it_copy);
			ite = _operators.end();
		}
	}
}

void	Channel::unbanUser(std::string str)
{
	std::vector<std::string>::iterator it = _banned.begin();
	std::vector<std::string>::iterator ite = _banned.end();
	std::vector<std::string>::iterator it_copy;
	for (; it != ite; it++)
	{
		if (sided_wildcompare(str, *it))
		{
			it_copy = it;
			it--;
			_banned.erase(it_copy);
			ite = _banned.end();
		}
	}
}

void	Channel::removeBanExcept(std::string str)
{
	std::vector<std::string>::iterator it = _ban_except.begin();
	std::vector<std::string>::iterator ite = _ban_except.end();
	std::vector<std::string>::iterator it_copy;
	for (; it != ite; it++)
	{
		if (sided_wildcompare(str, *it))
		{
			it_copy = it;
			it--;
			_ban_except.erase(it_copy);
			ite = _ban_except.end();
		}
	}
}

void	Channel::removeInviteExcept(std::string str)
{
	std::vector<std::string>::iterator it = _invite_except.begin();
	std::vector<std::string>::iterator ite = _invite_except.end();
	std::vector<std::string>::iterator it_copy;
	for (; it != ite; it++)
	{
		if (sided_wildcompare(str, *it))
		{
			it_copy = it;
			it--;
			_invite_except.erase(it_copy);
			ite = _invite_except.end();
		}
	}
}

void	Channel::removeVoiced(std::string str)
{
	std::vector<std::string>::iterator it = _voiced.begin();
	std::vector<std::string>::iterator ite = _voiced.end();
	std::vector<std::string>::iterator it_copy;
	for (; it != ite; it++)
	{
		if (sided_wildcompare(str, *it))
		{
			it_copy = it;
			it--;
			_voiced.erase(it_copy);
			ite = _voiced.end();
		}
	}
}

/* ----------ATTRIBUTE CHANGES---------- */

void	Channel::_setLimitMode(bool state)
{ _limit_mode = state; }

void	Channel::_setLimit(size_t limit)
{ _capacity = limit ; }

void	Channel::setLimit(bool state, size_t value)
{
	if (!_limit_mode && state)
		_modestring += "l";
	if (_limit_mode && !state)
		_modestring.erase(_modestring.find('l'), 1);
	_setLimitMode(state);
	_setLimit(value);
}

void	Channel::setInviteMode(bool state)
{
	if (!_invite_mode && state)
		_modestring += "i";
	if (_invite_mode && !state)
		_modestring.erase(_modestring.find('i'), 1);
	_invite_mode = state;
}

void	Channel::_setKeyMode(bool state)
{ _key_mode = state; }

void	Channel::_setKey(std::string new_key)
{ _key = new_key; }

void	Channel::setKey(bool state, std::string value)
{
	if (!_key_mode && state)
		_modestring += "k";
	if (_key_mode && !state)
		_modestring.erase(_modestring.find('k'), 1);
	if (state)
		_setKey(value);
	_setKeyMode(state);
}

void	Channel::setModeratedMode(bool state)
{
	if (!_moderated_mode && state)
		_modestring += "m";
	if (_moderated_mode && !state)
		_modestring.erase(_modestring.find('m'), 1);
	_moderated_mode = state;
}

void	Channel::setSecretMode(bool state)
{
	if (!_secret_mode && state)
		_modestring += "s";
	if (_secret_mode && !state)
		_modestring.erase(_modestring.find('s'), 1);
	_secret_mode = state;
}

void	Channel::setProtectedTopicMode(bool state)
{
	if (!_protected_topic_mode && state)
		_modestring += "t";
	if (_protected_topic_mode && !state)
		_modestring.erase(_modestring.find('t'), 1);
	_protected_topic_mode = state;
}

void	Channel::setNoExternalMessagesMode(bool state)
{
	if (!_no_external_messages_mode && state)
		_modestring += "n";
	if (_no_external_messages_mode && !state)
		_modestring.erase(_modestring.find('n'), 1);
	_no_external_messages_mode = state;
}

void	Channel::setTopic(std::string new_topic)
{ _topic = new_topic; }

void	Channel::unsetTopic()
{ _topic = "" ; }


/* ----------ATTRIBUTE CHECKS---------- */

bool	Channel::isMember(User *user) const
{ return std::find(_members.begin(), _members.end(), user) != _members.end() ; }

bool	Channel::_find_mask(std::vector<User *> vect, User * user) const
{
	std::vector<User *>::const_iterator it = vect.begin();
	std::vector<User *>::const_iterator ite = vect.end();
	for (; it != ite; it++)
	{
		if (wildcompare((*it)->getMask(), user->getMask()))
			return true;
	}
	return false;
}

bool	Channel::_find_mask(std::vector<std::string> vect, User * user) const
{
	std::vector<std::string>::const_iterator it = vect.begin();
	std::vector<std::string>::const_iterator ite = vect.end();
	for (; it != ite; it++)
	{
		if (wildcompare(*it, user->getMask()) || wildcompare(*it, user->getNickname()))
			return true;
	}
	return false;
}

bool	Channel::isFounder(User * user) const
{ return _founder == user ; }

bool	Channel::isOp(User *user) const
{ return isFounder(user) || _find_mask(_operators, user) ; }

bool	Channel::isBanned(User *user) const
{ return _find_mask(_banned, user) ; }

bool	Channel::isBanExcept(User *user) const
{ return _find_mask(_ban_except, user) ; }

bool	Channel::isInvited(User *user) const
{ return _find_mask(_invited, user) ; }

bool	Channel::isInviteExcept(User *user) const
{ return _find_mask(_invite_except, user) ; }

bool	Channel::isVoiced(User *user) const
{ return _find_mask(_voiced, user) ; }

bool	Channel::isFull() const
{ return _members.size() >= _capacity ; }

bool	Channel::checkKey(std::string s) const
{ return !(s.compare(_key)) ; }

bool	Channel::isTopicSet() const
{ return _topic.compare("") ; }

/* ----------COMMANDS---------- */

err_codes Channel::join(User *user, std::string s = "")
{
	// Mode checks

	if (keyMode())
	{
		if (!checkKey(s))
			return err_badchannelkey;
	}

	if (!isBanExcept(user) && isBanned(user))
		return err_bannedfromchan;

	if (limitMode())
	{
		if (isFull())
			return err_channelisfull;
	}

	if (inviteMode())
	{
		if (!isInviteExcept(user) && !isInvited(user))
			return err_inviteonlychan;
	}

	if (isMember(user))
		return err_useronchannel;

	// Adding user

	if (_members.empty())
		_founder = user;

	addMember(user);

	return err_noerror;
}

err_codes	Channel::part(User *user)
{
	if (!isMember(user))
		return err_notonchannel;
	
	std::vector<User *>::iterator it = std::find(_members.begin(), _members.end(), user);
	_members.erase(it);

	return err_noerror;
}

err_codes	Channel::changeTopic(User * user, std::string new_topic)
{
	if (!isMember(user))
		return err_notonchannel;

	if (protectedTopicMode() && !isOp(user))
		return err_chanoprivsneeded;

	_topic = new_topic;

	return err_noerror;
}

err_codes Channel::invite(User *from, User *to)
{
	if (!isMember(from))
		return err_notonchannel;
	
	if (inviteMode() && !isOp(from))
		return err_chanoprivsneeded;

	if (isMember(to))
		return err_useronchannel;

	addInvite(to);

	return err_noerror;
}

void Channel::kick(User *user)
{
	part(user);
}

std::vector<std::string> Channel::namesVect(User *user) const
{
	std::vector<std::string> name_vect;
	std::vector<User *>::const_iterator it;
	std::vector<User *>::const_iterator ite;

	it = _members.begin();
	ite = _members.end();

	for (; it != ite; it++)
	{
		User * current = *it;
		std::string current_name;

		if (current->isInvisible() && !isMember(user))
			continue;
		if (isOp(current))
			current_name = "@";
		else if (isVoiced(current))
			current_name = "+";
		else
			current_name = " ";
		current_name += current->getNickname();
		name_vect.push_back(current_name);
	}
	return name_vect;
}

std::string Channel::namesStr(User *user) const
{
	std::string ret;
	std::vector<std::string> vect = namesVect(user);

	for (size_t i = 0; i < vect.size(); i++)
	{
		ret += vect[i];
		if (i + 1 != vect.size())
			ret += ' ';
	}

	return ret;
}

void	Channel::sendToAllMembers(std::string msg) const
{
	std::vector<User *>::const_iterator it = _members.begin();
	std::vector<User *>::const_iterator ite = _members.end();

	msg += "\r\n";

	for (; it != ite; it++)
	{
		(*it)->getUserbuffer().append(msg);
	}
}

void	Channel::sendToAllMembers(std::string msg, int &userfd) const
{
	std::vector<User *>::const_iterator it = _members.begin();
	std::vector<User *>::const_iterator ite = _members.end();

	msg += "\r\n";

	for (; it != ite; it++)
	{
		if ((*it)->getFd() != userfd)
			(*it)->getUserbuffer().append(msg);
	}
}

void	Channel::up_creatime()
{ time(&this->_creatime); }

void	Channel::up_topictime()
{ time(&this->_topictime); }
