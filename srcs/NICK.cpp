#include "main.hpp"

void	nick(Server *srv, int &userfd, Command &cmd)
{
	// Number of args check
	std::string	client = "";
	if (cmd.paramNumber() == 0)
	{
		srv->sendReply(userfd, ERR_NONICKNAMEGIVEN(client));
		return;
	}

	// Check if nick not in use
	if (srv->getUserbyNickname(cmd.getParamList()[0]) != NULL)
	{
		srv->sendReply(userfd, ERR_NICKNAMEINUSE(client, cmd.getParamList()[0]));
		return;
	}

	/* Need to check here if there are disallowed characters */
	srv->getUser(userfd)->setNickname(cmd.getParam(0));
	srv->getUser(userfd)->setNick(true);
	if (srv->getUser(userfd)->checkAuth() == true && srv->getUser(userfd)->getAuth() == false)
	{
		srv->getUser(userfd)->setAuth(true);
		srv->sendReply(userfd, ":" + srv->getHostname() + " " + RPL_WELCOME(srv->getUser(userfd)->getNickname()));
	}
}
