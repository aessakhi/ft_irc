#include "main.hpp"

void	whowas(Server *srv, int &userfd, Command &cmd)
{ //IDK if will do or not but on ERR from WHOIS, client SEND to WHOWAS
	User * user = srv->getUser(userfd);

	if (cmd.paramNumber() == 0)
	{
		srv->sendReply(userfd, ERR_NEEDMOREPARAMS(user->getNickname(), cmd.getCmd()));
		return;
	}

	srv->sendReply(userfd, ERR_NOSUCHNICK(user->getNickname(), cmd.getParam(0)));
	srv->sendReply(userfd, RPL_ENDOFWHOWAS(user->getNickname(), cmd.getParam(0)));
}