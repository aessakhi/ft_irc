#include "main.hpp"

void	nick(Server *srv, int &userfd, Command &cmd)
{
	std::string	client;

	client = "";
	if (cmd.getParamList().empty())
		srv->sendReply(userfd, ERR_NONICKNAMEGIVEN(client));
	else if (srv->getUserbyNickname(cmd.getParamList()[0]) != NULL)
		srv->sendReply(userfd, ERR_NICKNAMEINUSE(client, cmd.getParamList()[0]));
	else
	{
		/* Need to check here if there are disallowed characters */
		srv->getUser(userfd)->setNickname(cmd.getParamList()[0]);
		srv->getUser(userfd)->setNick(true);
		if (srv->getUser(userfd)->checkAuth() == true && srv->getUser(userfd)->getAuth() == false)
		{
			srv->getUser(userfd)->setAuth(true);
			srv->sendReply(userfd, RPL_WELCOME(srv->getUser(userfd)->getNickname()));
		}
	}
}
