#include "main.hpp"

/*
https://dev.to/nichartley/how-do-you-use-irc-311f

It can be at most nine-characters long, and can contain :
letters (uppercase or lowercase)
numbers
hyphens (-)
underscores (_)
square brackets ([])
curly braces ({})
backslash (\)
backticks (`)
pipes (|)
*/

static bool _is_allowed_char(char c)
{
	return (isalnum(c) || (std::string("-_[]{}\\`|").find(c) != std::string::npos));
}

static bool _is_allowed_nick(std::string s)
{
	if (s.size() > 9)
		return false;
	for (size_t i = 0; i < s.size(); i++)
	{
		if (!_is_allowed_char(s[i]))
			return false;
	}
	return true;
}

void	nick(Server *srv, int &userfd, Command &cmd)
{
	User	*user;

	user = srv->getUser(userfd);
	if (!user)
		return ;
	if (!user->getPwd())
	{
		quit(srv, userfd, cmd);
		return ;
	}
	// Number of args check
	if (cmd.paramNumber() == 0)
	{
		srv->sendReply(userfd, ERR_NONICKNAMEGIVEN(user->getNickname()));
		return;
	}

	std::string requested_nick = cmd.getParam(0);

	// Check if nick has proper format
	if (!_is_allowed_nick(requested_nick))
	{
		if (user->getNickname().empty())
			srv->sendReply(userfd, ERR_ERRONEUSNICKNAME(requested_nick, requested_nick));
		else
			srv->sendReply(userfd, ERR_ERRONEUSNICKNAME(user->getNickname(), requested_nick));
		return;
	}

	// Check if nick not in use
	if (srv->getUserbyNickname(requested_nick) != NULL)
	{
		if (user->getNickname().empty())
			srv->sendReply(userfd, ERR_NICKNAMEINUSE(requested_nick, requested_nick));
		else
			srv->sendReply(userfd, ERR_NICKNAMEINUSE(user->getNickname(), requested_nick));
		return;
	}

	// If the nickname was changed (i.e. if user had a nickname before), message all server users to inform them of the change
	if (user->getNick())
	{
		std::map<int, User *> user_map = srv->getUserMap();
		std::map<int, User *>::const_iterator it = user_map.begin();
		std::map<int, User *>::const_iterator ite = user_map.end();
		for (; it != ite; it++)
		{
			srv->sendReply(it->first, ":" + user->getMask() + " NICK " + requested_nick);
		}
		user->setOldnick(user->getNickname());
		user->setNickname(requested_nick);
		return ;
	}

	user->setOldnick(user->getNickname());
	user->setNickname(requested_nick);
	user->setNick(true);
	if (user->checkAuth() && !user->getAuth())
	{
		user->setAuth(true);
		srv->sendReply(userfd, ":" + srv->getHostname() + " " + RPL_WELCOME(user->getNickname()));
		return ;
	}
	if (user->getAuth())
	{
		srv->sendReply(userfd, ":" + user->getOldNick() + "!" + user->getUsername() + "@" + user->getHostname() + " NICK " + user->getNickname());
		return ;
	}
}
