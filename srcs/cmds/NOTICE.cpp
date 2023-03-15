#include "main.hpp"

static bool _error_check(Command &cmd)
{
	return (cmd.paramNumber() <= 1 || cmd.getParam(0).empty());
}

void	_notice_to_channel(Server *srv, User * user, std::string target, std::string msg)
{
	Channel * channel = srv->getChannel(target);
	if (channel == NULL)
		return;
	
	channel->sendToAllMembers(":" + user->getMask() + " " + msg);
}

void	_notice_to_user(Server *srv, User * user, std::string target, std::string msg)
{
	int dest_fd = srv->getUserfd(target);
	if (dest_fd == -1)
		return;
	srv->sendReply(dest_fd, ":" + user->getMask() + " " + msg);
}

void	notice(Server *srv, int &userfd, Command &cmd)
{
	if (_error_check(cmd))
		return ;

	User * user = srv->getUser(userfd);
	std::string msg = cmd.getParam(1);
	std::vector<std::string> targets = actual_split(cmd.getParam(0), ",");
	std::vector<std::string>::const_iterator it = targets.begin();
	std::vector<std::string>::const_iterator ite = targets.end();
	for (; it != ite; it++)
	{
		if (it->empty())
			continue;
		if (it->at(0) == '#')
			_notice_to_channel(srv, user, *it, msg);
		else
			_notice_to_user(srv, user, *it, msg);
	}
}