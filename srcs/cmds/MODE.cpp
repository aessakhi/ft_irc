#include "main.hpp"

void	usermode(Server *srv, int &userfd, Command &cmd);
void	channelmode(Server *srv, int &userfd, Command &cmd);

void	mode(Server *srv, int &userfd, Command &cmd)
{
	if (cmd.paramNumber() == 0)
	{
		srv->sendReply(userfd, ERR_NEEDMOREPARAMS(srv->getUser(userfd)->getNickname(), cmd.getCmd()));
		return;
	}

	if (cmd.getParam(0)[0] == '#')
		channelmode(srv, userfd, cmd);
	else
		usermode(srv, userfd, cmd);
}
