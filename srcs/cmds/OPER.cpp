#include "main.hpp"

void	oper(Server * srv, int & userfd, Command & cmd)
{
	User * user = srv->getUser(userfd);

	if (cmd.paramNumber() < 2)
	{
		srv->sendReply(userfd, ERR_NEEDMOREPARAMS(user->getNickname(), cmd.getCmd()));
		return;
	}

	if (!srv->check_oper_password(cmd.getParam(1)))
	{
		srv->sendReply(userfd, ERR_PASSWDMISMATCH(user->getNickname()));
		return;
	}

	user->setOperator(true);
	srv->sendReply(userfd, RPL_YOUREOPER(user->getNickname()));
}