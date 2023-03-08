#include "main.hpp"

void	mode(Server *srv, int &userfd, Command &cmd)
{
	if (cmd.paramNumber() == 0)
	{
		srv->sendReply(userfd, ERR_NEEDMOREPARAMS(srv->getUser(userfd)->getNickname(), cmd.getCmd()));
		return;
	}

	if (cmd.getParam(0)[0] == '#')
		_channelmode(srv, userfd, cmd);
	else
		_usermode(srv, userfd, cmd);
}
