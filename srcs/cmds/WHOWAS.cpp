#include "main.hpp"

void	whowas(Server *srv, int &userfd, Command &cmd)
{ //IDK if will do or not but on ERR from WHOIS, client SEND to WHOWAS
	User * user = srv->getUser(userfd);

	if (cmd.paramNumber() == 0)
	{
		srv->sendReply(userfd, ERR_NEEDMOREPARAMS(user->getNickname(), cmd.getCmd()));
		return;
	}

	std::string target = cmd.getParam(0);
	User * target_user = srv->getUserbyNickname(target);
	if (target_user == NULL)
	{
		srv->sendReply(userfd, ERR_NOSUCHNICK(user->getNickname(), target));
	}
	else
	{
		srv->sendReply(userfd, RPL_WHOWASUSER(user->getNickname(), target, target_user->getUsername(), target_user->getHostname(), target_user->getRealname()));
	}
	
	srv->sendReply(userfd, RPL_ENDOFWHOWAS(user->getNickname(), target));
}