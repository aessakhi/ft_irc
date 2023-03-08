#include "main.hpp"

static std::vector<std::string> actual_split(std::string str, std::string delimiter)
{
	std::vector<std::string> result;

	size_t	start = 0;
	size_t	end = str.find(delimiter);

	while (end != std::string::npos)
	{
		result.push_back(str.substr(start, end));
		start = end + delimiter.length();
		end = str.find(delimiter, start);
	}
	result.push_back(str.substr(start, end));
	return (result);
}

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

	std::string reply_str = ":" + user->getNickname() + " PART " + channel_name;
	if (cmd.paramNumber() > 1)
		reply_str += " " + cmd.getParam(1);

	channel->sendToAllMembers(reply_str);

	channel->part(user);
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