#include "main.hpp"

void	version(Server *srv, int &userfd, Command &cmd)
{
	(void)cmd;
	User * user = srv->getUser(userfd);

	srv->sendReply(userfd, RPL_VERSION(user->getNickname(), "1.0", srv->getName(), ""));
}