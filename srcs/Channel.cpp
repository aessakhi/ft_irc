#include "Channel.hpp"

Channel::Channel(const std::string & name) : _name(name), _modestring(""), _limit_mode(false), _invite_mode(false), _key_mode(false), _moderated_mode(false), _secret_mode(false), _protected_topic_mode(false), _no_external_messages_mode(false) 
{}

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
		ret += ' ' + _capacity;
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

std::vector<UserMask>	Channel::getOperators() const
{ return _operators ; }

std::vector<UserMask>	Channel::getBanned() const
{ return _banned ; }

std::vector<UserMask>	Channel::getBanExcept() const
{ return _ban_except ; }

std::vector<UserMask>	Channel::getInviteExcept() const
{ return _invite_except ; }

std::vector<UserMask>	Channel::getVoiced() const
{ return _voiced ; }

/* ----------ADDING USERS TO LISTS---------- */

void	Channel::addMember(User *user)
{ _members.push_back(user); }

void	Channel::addOperator(UserMask user)
{ _operators.push_back(user); }

void	Channel::banUser(UserMask user)
{ _banned.push_back(user); }

void	Channel::addBanExcept(UserMask user)
{ _ban_except.push_back(user); }

void	Channel::addInviteExcept(UserMask user)
{ _invite_except.push_back(user); }

void	Channel::addInvite(User * user)
{ _invited.push_back(user) ; }

void	Channel::addVoiced(UserMask user)
{ _voiced.push_back(user) ; }

/* ----------ATTRIBUTE CHANGES---------- */

void	Channel::_setLimitMode(bool state)
{ _limit_mode = state; }

void	Channel::_setLimit(size_t limit)
{ _capacity = limit ; }

void	Channel::setLimit(bool state, size_t value)
{
	if (!_limit_mode)
		_modestring += "l";
	_setLimitMode(state);
	_setLimit(value);
}

void	Channel::setInviteMode(bool state)
{
	if (!_invite_mode)
		_modestring += "i";
	_invite_mode = state;
}

void	Channel::_setKeyMode(bool state)
{ _key_mode = state; }

void	Channel::_setKey(std::string new_key)
{ _key = new_key; }

void	Channel::setKey(bool state, std::string value)
{
	if (!_key_mode)
		_modestring += "k";
	_setKeyMode(state);
	_setKey(value);
}

void	Channel::setModeratedMode(bool state)
{
	if (!_moderated_mode)
		_modestring += "m";
	_moderated_mode = state;
}

void	Channel::setSecretMode(bool state)
{
	if (!_secret_mode)
		_modestring += "s";
	_secret_mode = state;
}

void	Channel::setProtectedTopicMode(bool state)
{
	if (!_protected_topic_mode)
		_modestring += "t";
	_protected_topic_mode = state;
}

void	Channel::setNoExternalMessagesMode(bool state)
{
	if (!_no_external_messages_mode)
		_modestring += "n";
	_no_external_messages_mode = state;
}

void	Channel::setTopic(std::string new_topic)
{ _topic = new_topic; }

void	Channel::unsetTopic()
{ _topic = "" ; }


/* ----------ATTRIBUTE CHECKS---------- */

bool	Channel::isMember(User *user) const
{ return std::find(_members.begin(), _members.end(), user) == _members.end() ; }

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

bool	Channel::_find_mask(std::vector<UserMask> vect, User * user) const
{
	std::vector<UserMask>::const_iterator it = vect.begin();
	std::vector<UserMask>::const_iterator ite = vect.end();
	for (; it != ite; it++)
	{
		if (wildcompare(it->getMask(), user->getMask()))
			return true;
	}
	return false;
}

bool	Channel::isOp(User *user) const
{ return _find_mask(_operators, user) ; }

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
{ return _members.size() == _capacity ; }

bool	Channel::checkKey(std::string s) const
{ return !(s.compare(_key)) ; }

bool	Channel::isTopicSet() const
{ return !(_topic.compare("")) ; }

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

	// Adding user

	if (_members.empty())
		addOperator(user);
	addMember(user);

	return err_noerror;
}

err_codes	Channel::part(User *user)
{
	if (!isMember(user))
		return err_notonchannel;
	
	std::remove(_members.begin(), _members.end(), user);

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

err_codes Channel::kick(User *from, User *to)
{
	if (!isMember(from))
		return err_notonchannel;
	
	if (!isOp(from))
		return err_chanoprivsneeded;

	if (!isMember(to))
		return err_usernotinchannel;
	
	std::remove(_members.begin(), _members.end(), to);

	return err_noerror;
}

std::vector<std::string> Channel::namesVect(User *user) const
{
	std::vector<std::string> name_vect;
	std::vector<User *>::const_iterator it;
	std::vector<User *>::const_iterator ite;

	bool see_invisible = isMember(user);

	it = _members.begin();
	ite = _members.end();

	for (; it != ite; it++)
	{
		User * current = *it;
		std::string current_name;

		if (current->isInvisible() && !see_invisible)
			continue;
		if (isOp(current))
			current_name += '@';
		else if (isVoiced(current))
			current_name += '+';
		else
			current_name += ' ';
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