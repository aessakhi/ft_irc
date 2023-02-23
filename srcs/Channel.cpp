#include "Channel.hpp"

Channel::Channel(const std::string & name) : _name(name)
{}

Channel::~Channel() {}

/* ----------ACCESSORS---------- */

const std::string & Channel::getName() const
{ return _name ; }

const std::string & Channel::getTopic() const
{ return _topic ; }

bool	Channel::banMode() const
{ return _ban_mode ; }

bool	Channel::banExceptMode() const
{ return _ban_except_mode ; }

bool	Channel::limitMode() const
{ return _limit_mode ; }

bool	Channel::inviteMode() const
{ return _invite_mode ; }

bool	Channel::inviteExceptMode() const
{ return _invite_except_mode ; }

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

void	Channel::addVoice(UserMask user)
{ _voice.push_back(user) ; }

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

bool	Channel::isFull() const
{ return _members.size() == _capacity ; }

bool	Channel::checkKey(std::string s) const
{ return !(s.compare(_key)) ; }

bool	Channel::isTopicSet() const
{ return _topic_is_set ; }

/* ----------COMMANDS---------- */

err_codes Channel::join(User *user, std::string s = "")
{
	// Mode checks

	if (keyMode())
	{
		if (!checkKey(s))
			return err_badchannelkey;
	}

	if (banMode())
	{
		if (banExceptMode() && !isBanExcept(user) && isBanned(user))
			return err_bannedfromchan;
		
		if (!banExceptMode() && isBanned(user))
			return err_bannedfromchan;
	}

	if (limitMode())
	{
		if (isFull())
			return err_channelisfull;
	}

	if (inviteMode())
	{
		if (inviteExceptMode() && !isInviteExcept(user) && !isInvited(user))
			return err_inviteonlychan;
		
		if (!inviteExceptMode() && !isInvited(user))
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
	_topic_is_set = true;

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
		if (i + 1 == vect.size())
			ret += ' ';
	}

	return ret;
}