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

static bool _error_check(Server * srv, int & userfd, Command & cmd)
{
	User * user = srv->getUser(userfd);

	if (cmd.paramNumber() < 2)
	{
		srv->sendReply(userfd, ERR_NEEDMOREPARAMS(user->getNickname(), cmd.getCmd()));
		return true;
	}

	if (!user->isOperator())
	{
		srv->sendReply(userfd, ERR_NOPRIVILEGES(user->getNickname()));
		return true;
	}

	return false;
}

static std::string _killed_str(User * killer, std::string reason)
{
	return "Killed (" + killer->getNickname() + " (" + reason + "))";
}

void	kill(Server * srv, int & userfd, Command & cmd)
{
	if (_error_check(srv, userfd, cmd))
		return;

	User * killer = srv->getUser(userfd);
	std::string target_nick = cmd.getParam(0);
	std::string reason = cmd.getParam(1);
	User * target_user = srv->getUserbyNickname(target_nick);
	int target_fd = srv->getUserfd(target_nick);
	if (target_user == NULL)
	{
		srv->sendReply(userfd, ERR_NOSUCHNICK(killer->getNickname(), target_nick));
		return;
	}

	srv->sendReply(target_fd, ":" + killer->getNickname() + " KILL " + target_nick + " " + reason);

	std::vector<std::string> empty_chans;

	std::map<std::string, Channel *> channels = *(srv->getChannelMap());
	std::map<std::string, Channel *>::iterator it = channels.begin();
	std::map<std::string, Channel *>::iterator ite = channels.end();
	for (; it != ite; it++)
	{
		if (!it->second->part(target_user))
			it->second->sendToAllMembers(":" + target_user->getMask() + " QUIT :" + _killed_str(killer, reason));
		if (it->second->getUsers().empty())
			empty_chans.push_back(it->first);
	}

	srv->sendReply(target_fd, ":" + srv->getName() + " ERROR :Closing Link: " + srv->getName() + "(" + _killed_str(killer, reason));

	srv->_removeUserfromServer(target_fd);

	_destroy_chans(srv, empty_chans);
}