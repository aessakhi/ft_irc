#include "main.hpp"

void	away(Server *srv, int &userfd, Command &cmd)
{
	if (cmd.getParamList().empty() || cmd.getParam(0).empty())
	{
		srv->getUser(userfd)->setAway(false, "");
		srv->sendReply(userfd, ":" + srv->getHostname() + " " + RPL_UNAWAY(srv->getUser(userfd)->getNickname()));
		return ;
	}
	else
	{
		srv->getUser(userfd)->setAway(true, cmd.getParam(0));
		srv->sendReply(userfd, ":" + srv->getHostname() + " " + RPL_NOWAWAY(srv->getUser(userfd)->getNickname()));
		return ;
	}
}
