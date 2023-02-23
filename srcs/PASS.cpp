#include "main.hpp"

void	pass(Server *srv, int &userfd, Command &cmd)
{
	std::string user = "New User";

	// Number of args check
	if (cmd.paramNumber() == 0)
	{
		srv->sendReply(userfd, ERR_NEEDMOREPARAMS(user, cmd.getCmd()));
		return;
	}

	// Check if password is correct
	if (cmd.getParam(0).compare(srv->getpwd()) != 0)
	{
		srv->sendReply(userfd, ERR_PASSWDMISMATCH(user));
		/* Might need to kill connection to user here */
		return;
	}

	srv->getUser(userfd)->setPwd(true);
	if (srv->getUser(userfd)->checkAuth() == true && srv->getUser(userfd)->getAuth() == false)
	{
		srv->getUser(userfd)->setAuth(true);
		srv->sendReply(userfd, ":" + srv->getHostname() + " " + RPL_WELCOME(srv->getUser(userfd)->getNickname()));
	}
}
