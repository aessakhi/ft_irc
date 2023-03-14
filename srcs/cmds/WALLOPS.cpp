#include "main.hpp"

void	wallops(Server * srv, int & userfd, Command & cmd)
{
	User * user = srv->getUser(userfd);

	if (cmd.paramNumber() == 0)
	{
		srv->sendReply(userfd, ERR_NEEDMOREPARAMS(user->getNickname(), cmd.getCmd()));
		return;
	}

	if (!user->isOperator())
	{
		srv->sendReply(userfd, ERR_NOPRIVILEGES(user->getNickname()));
		return;
	}

	std::string msg = ":" + user->getMask() + " WALLOPS " + cmd.getParam(0);
	std::map<int, User *> user_map = srv->getUserMap();
	std::map<int, User *>::const_iterator it = user_map.begin();
	std::map<int, User *>::const_iterator ite = user_map.end();
	for (; it != ite; it++)
	{
		if (it->second->isWallops())
			srv->sendReply(it->second->getFd(), msg);
	}
	if (!user->isWallops())
		srv->sendReply(userfd, msg);
}