#include "main.hpp"

//Connecion to localhost established, but user not welcomed then still receives arg

void	quit(Server *srv, int &userfd, Command &cmd)
{
	User * user = srv->getUser(userfd);
	std::string reason;

	if (cmd.paramNumber() >= 1)
		reason = cmd.getLastParam();

	srv->sendReply(userfd, ":" + srv->getName() + " ERROR :Quitting");

	std::map<std::string, Channel *> channels = *(srv->getChannelMap());
	std::map<std::string, Channel *>::iterator it = channels.begin();
	std::map<std::string, Channel *>::iterator ite = channels.end();
	for (; it != ite; it++)
	{
		if (!it->second->part(user))
			it->second->sendToAllMembers(":" + user->getMask() + " QUIT :Quit: " + reason);
	}

	srv->_removeUserfromServer(userfd);
}