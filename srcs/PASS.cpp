#include "main.hpp"

void	pass(Server *srv, int &userfd, Command &cmd)
{
	std::string user = "New User";
	std::string cmdName = "PASS";

	if (cmd.getParamList().empty())
		srv->sendReply(userfd, ERR_NEEDMOREPARAMS(user, cmdName));
	else if (cmd.getParamList()[0].compare(srv->getpwd()) != 0)
	{
		srv->sendReply(userfd, ERR_PASSWDMISMATCH(user));
		/* Might need to kill connection to user here */
	}
	else if (cmd.getParamList()[0].compare(srv->getpwd()) == 0)
	{
		srv->getUser(userfd)->setPwd(true);
		if (srv->getUser(userfd)->checkAuth() == true && srv->getUser(userfd)->getAuth() == false)
		{
			srv->getUser(userfd)->setAuth(true);
			srv->sendReply(userfd, RPL_WELCOME(srv->getUser(userfd)->getNickname()));
		}
	}
}
