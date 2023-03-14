#include "main.hpp"

static void	part_one_chan(Server *srv, int &userfd, Command &cmd, std::string channel_name)
{
	User * user = srv->getUser(userfd);
	Channel * channel = srv->getChannel(channel_name);

	if (channel == NULL)
	{
		srv->sendReply(userfd, ERR_NOSUCHCHANNEL(user->getNickname(), channel_name));
		return;
	}

	if (!channel->isMember(user))
	{
		srv->sendReply(userfd, ERR_NOTONCHANNEL(user->getNickname(), channel_name));
		return;
	}

	std::string reply_str = ":" + user->getMask() + " PART " + channel_name;
	if (cmd.paramNumber() > 1)
		reply_str += " " + cmd.getParam(1);

	channel->sendToAllMembers(reply_str);

	channel->part(user);

	if (channel->getUsers().empty())
	{
		srv->getChannelMap()->erase(channel_name);
		delete channel;
	}
}

void	part(Server *srv, int &userfd, Command &cmd)
{
	User * user = srv->getUser(userfd);

	if (cmd.paramNumber() == 0)
	{
		srv->sendReply(userfd, ERR_NEEDMOREPARAMS(user->getNickname(), cmd.getCmd()));
		return ;
	}

	std::vector<std::string> splitted = actual_split(cmd.getParam(0), ",");

	std::vector<std::string>::const_iterator it = splitted.begin();
	std::vector<std::string>::const_iterator ite = splitted.end();
	for (; it != ite; it++)
	{
		part_one_chan(srv, userfd, cmd, *it);
	}
}