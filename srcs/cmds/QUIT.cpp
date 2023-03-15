#include "main.hpp"

static void _destroy_chans(Server * srv, std::vector<std::string> to_delete)
{
	std::map<std::string, Channel *> *channels = srv->getChannelMap();
	std::map<std::string, Channel *>::iterator pair;
	std::vector<std::string>::const_iterator it = to_delete.begin();
	std::vector<std::string>::const_iterator ite = to_delete.end();
	for (; it != ite; it ++)
	{
		if (channels->find(*it) != channels->end())
		{
			pair = channels->find(*it);
			delete pair->second;
			channels->erase(*it);
		}
	}
}

//Connecion to localhost established, but user not welcomed then still receives arg

void	quit(Server *srv, int &userfd, Command &cmd)
{
	User * user = srv->getUser(userfd);
	std::string reason;

	if (cmd.paramNumber() >= 1)
		reason = cmd.getLastParam();

	srv->sendReply(userfd, ":" + srv->getName() + " ERROR :Quitting");

	std::vector<std::string> empty_chans;

	std::map<std::string, Channel *> channels = *(srv->getChannelMap());
	std::map<std::string, Channel *>::iterator it = channels.begin();
	std::map<std::string, Channel *>::iterator ite = channels.end();
	for (; it != ite; it++)
	{
		if (!it->second->part(user))
			it->second->sendToAllMembers(":" + user->getMask() + " QUIT :Quit: " + reason);
		if (it->second->getUsers().empty())
			empty_chans.push_back(it->first);
	}

	srv->_removeUserfromServer(userfd);

	_destroy_chans(srv, empty_chans);
}