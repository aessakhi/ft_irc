#include "main.hpp"

void	names(Server *srv, int &userfd, Command &cmd)
{
	User * user = srv->getUser(userfd);
	std::string client_name = user->getNickname();

	if (cmd.paramNumber() == 0)
	{
		srv->sendReply(userfd, ERR_NEEDMOREPARAMS(client_name, cmd.getCmd()));
		return ;
	}

	std::vector<std::string> channels = actual_split(cmd.getParam(0), ",");
	std::vector<std::string>::const_iterator it = channels.begin();
	std::vector<std::string>::const_iterator ite = channels.end();
	Channel * channel;
	for (; it != ite; it++)
	{
		channel = srv->getChannel(*it);

		// invalid channel or secret channel that user hasnt joined
		if (channel == NULL || (channel->secretMode() && !channel->isMember(user)))
		{
			srv->sendReply(userfd, RPL_ENDOFNAMES(client_name, *it));
			continue;
		}
		
		char symbol = (channel->secretMode() ? '@' : '=');

		srv->sendReply(userfd, RPL_NAMREPLY(client_name, symbol, *it, channel->namesStr(user)));
		srv->sendReply(userfd, RPL_ENDOFNAMES(client_name, *it));
	}
}