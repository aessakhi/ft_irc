#include "main.hpp"

void	user(Server *srv, int &userfd, Command &cmd)
{

	// Number of args check
	std::string client = "";
	if (cmd.paramNumber() < 4)
	{
		if (srv->getUser(userfd)->getNick() == true)
			srv->sendReply(userfd, ERR_NEEDMOREPARAMS(srv->getUser(userfd)->getNickname(), cmd.getCmd()));
		else
			srv->sendReply(userfd, ERR_NEEDMOREPARAMS(client, cmd.getCmd()));

		return ;
	}

	// Registration of user

	srv->getUser(userfd)->setUsername(cmd.getParam(0));
	srv->getUser(userfd)->setRealname(cmd.getLastParam());
	srv->getUser(userfd)->setUser(true);

	if (srv->getUser(userfd)->checkAuth() == true && srv->getUser(userfd)->getAuth() == false)
	{
		srv->getUser(userfd)->setAuth(true);
		srv->sendReply(userfd, ":" + srv->getHostname() + " " + RPL_WELCOME(srv->getUser(userfd)->getNickname()));
	}
}
