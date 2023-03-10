#include "main.hpp"

void	who(Server * srv, int & userfd, Command & cmd)
{
	User * user = srv->getUser(userfd);

	if (cmd.paramNumber() == 0)
	{
		srv->sendReply(userfd, ERR_NEEDMOREPARAMS(user->getNickname(), cmd.getCmd()));
		return;
	}

	std::string target = cmd.getParam(0);

	// WHO for channels ==> return list
	if (target[0] == '#')
	{
		Channel * channel = srv->getChannel(target);
		if (channel)
		{
			std::vector<User *> user_list = channel->getUsers();
			std::vector<User *>::const_iterator it = user_list.begin();
			std::vector<User *>::const_iterator ite = user_list.end();
			User * current;
			std::string flags;
			for (; it != ite; it++)
			{
				current = *it;
				if (!current->isInvisible())
				{
					// set flags here
					srv->sendReply(userfd, RPL_WHOREPLY(user->getNickname(), target, current->getUsername(), current->getHostname(), srv->getName(), current->getNickname(), flags, "0", current->getRealname()));
				}
			}
		}
		srv->sendReply(userfd, RPL_ENDOFWHO(user->getNickname(), target));
		return;
	}

	// WHO for single user ==> return infos
}