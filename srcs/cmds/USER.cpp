#include "main.hpp"

void	user(Server *srv, int &userfd, Command &cmd)
{
	User	*user;

	user = srv->getUser(userfd);
	if (!user)
		return ;
	// Number of args check
	std::string client = "";
	if (!user->getPwd())
	{
		quit(srv, userfd, cmd);
		return ;
	}
	if (!user->getNickname().empty())
		client = user->getNickname();
	if (user->getAuth())
	{
		srv->sendReply(userfd, ":" + srv->getHostname() + " " + ERR_ALREADYREGISTERED(client));
		return ;
	}
	if (cmd.paramNumber() < 4)
	{
		srv->sendReply(userfd, ERR_NEEDMOREPARAMS(client, cmd.getCmd()));
		return ;
	}
	// Registration of user
	user->setUsername(cmd.getParam(0));
	user->setRealname(cmd.getLastParam());
	user->setUser(true);

	if (user->checkAuth() && !user->getAuth())
	{
		user->setAuth(true);
		srv->sendReply(userfd, ":" + srv->getHostname() + " " + RPL_WELCOME(client));
	}
}
