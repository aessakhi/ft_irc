#include "main.hpp"

static std::string _get_user_flags(User * user, Channel * channel = NULL)
{
	std::string flags;

	if (user->isAway())
		flags = "G";
	else
		flags = "H";
	if (user->isOperator())
		flags += "*";
	if (channel)
	{
		if (channel->isFounder(user))
			flags += "~";
		else if (channel->isOp(user))
			flags += "@";
		else if (channel->isVoiced(user))
			flags += "+";
	}

	return flags ;
}

void	who(Server * srv, int & userfd, Command & cmd)
{
	User * user = srv->getUser(userfd);

	if (cmd.paramNumber() == 0)
	{
		srv->sendReply(userfd, ERR_NEEDMOREPARAMS(user->getNickname(), cmd.getCmd()));
		return;
	}

	std::string target = cmd.getParam(0);
	User * requested;

	// WHO for channels ==> return list
	if (target[0] == '#')
	{
		Channel * channel = srv->getChannel(target);
		if (channel)
		{
			std::vector<User *> user_list = channel->getUsers();
			std::vector<User *>::const_iterator it = user_list.begin();
			std::vector<User *>::const_iterator ite = user_list.end();
			
			for (; it != ite; it++)
			{
				requested = *it;
				if (!requested->isInvisible())
				{
					srv->sendReply(userfd, RPL_WHOREPLY(user->getNickname(), target, requested->getUsername(), requested->getHostname(), srv->getName(), requested->getNickname(), _get_user_flags(requested, channel), "1", requested->getRealname()));
				}
			}
		}
		srv->sendReply(userfd, RPL_ENDOFWHO(user->getNickname(), target));
		return;
	}

	// WHO for single user ==> return infos
	requested = srv->getUserbyNickname(target);
	if (requested)
	{
		if (!requested->isInvisible())
		{
			srv->sendReply(userfd, RPL_WHOREPLY(user->getNickname(), "*", requested->getUsername(), requested->getHostname(), srv->getName(), requested->getNickname(), _get_user_flags(requested), "1", requested->getRealname()));
		}
	}
	srv->sendReply(userfd, RPL_ENDOFWHO(user->getNickname(), target));
}