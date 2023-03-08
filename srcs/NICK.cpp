#include "main.hpp"

void	nick(Server *srv, int &userfd, Command &cmd)
{
	User	*user;

	user = srv->getUser(userfd);
	if (!user)
		return ;
	// Number of args check
	if (cmd.paramNumber() == 0)
	{
		srv->sendReply(userfd, ERR_NONICKNAMEGIVEN(cmd.getParam(0)));
		return;
	}

	// Check if nick not in use
	if (srv->getUserbyNickname(cmd.getParamList()[0]) != NULL)
	{
		if (user->getNickname().empty())
			srv->sendReply(userfd, ERR_NICKNAMEINUSE(cmd.getParam(0), cmd.getParam(0)));
		else
			srv->sendReply(userfd, ERR_NICKNAMEINUSE(user->getNickname(), cmd.getParam(0)));
		return;
	}

	/* Need to check here if there are disallowed characters */

	user->setOldnick(user->getNickname());
	user->setNickname(cmd.getParam(0));
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
