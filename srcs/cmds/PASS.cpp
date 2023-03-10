#include "main.hpp"

void	pass(Server *srv, int &userfd, Command &cmd)
{
	User	*user = srv->getUser(userfd);
	std::string client = "";

	if (!user->getNickname().empty())
		client = user->getNickname();
	if (user->getAuth())
	{
		srv->sendReply(userfd, ":" + srv->getHostname() + " " + ERR_ALREADYREGISTERED(user->getNickname()));
		return ;
	}
	// Number of args check
	if (cmd.paramNumber() == 0)
	{
		srv->sendReply(userfd, ERR_NEEDMOREPARAMS(client, cmd.getCmd()));
		return;
	}

	// Check if password is correct
	if (cmd.getParam(0).compare(srv->getpwd()) != 0)
	{
		srv->sendReply(userfd, ERR_PASSWDMISMATCH(client));
		quit(srv, userfd, cmd);
		/* Might need to kill connection to user here */
		return; //return but the user has been killed and deleted so it segfaults on nick
	}

	user->setPwd(true);
	if (user->checkAuth() == true && user->getAuth() == false)
	{
		user->setAuth(true);
		srv->sendReply(userfd, ":" + srv->getHostname() + " " + RPL_WELCOME(user->getNickname()));
	}
}
