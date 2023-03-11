#include "main.hpp"
#include <utility>

static std::vector<std::string>	splitTargets(std::string str, std::string delimiter)
{
	std::vector<std::string> result;

	size_t	end = str.find(delimiter);

	while (end != std::string::npos)
	{
		result.push_back(str.substr(0, end));
		str.erase(0, end + delimiter.length());
		end = str.find(delimiter);
	}
	result.push_back(str);
	return (result);
}

void	join(Server *srv, int &userfd, Command &cmd)
{
	std::vector<std::string> channels;
	std::vector<std::string> keys;
	err_codes	err;
	User		*user;

	user = srv->getUser(userfd);
	if (cmd.getParamList().empty())
	{
		srv->sendReply(userfd, ERR_NEEDMOREPARAMS(user->getNickname(), "JOIN"));
		return ;
	}
	channels = splitTargets(cmd.getParam(0), ",");
	if (cmd.getParamList().size() > 1)
		keys = splitTargets(cmd.getParam(1), ",");
	while (keys.size() < channels.size())
		keys.push_back("");
	for (std::vector<std::string>::const_iterator it = channels.begin(); it != channels.end(); it++)
	{
		if (srv->getChannel(*it) == NULL)
			srv->getChannelMap()->insert(std::make_pair(*it, new Channel(*it)));
		Channel * channel = srv->getChannel(*it);
		err = channel->join(user, keys[it - channels.begin()]);
		switch (err)
		{
			case err_badchannelkey:
				srv->sendReply(userfd, ERR_BADCHANNELKEY(user->getNickname(), *it));
				break;
			case err_bannedfromchan:
				srv->sendReply(userfd, ERR_BANNEDFROMCHAN(user->getNickname(), *it));
				break;
			case err_channelisfull:
				srv->sendReply(userfd, ERR_CHANNELISFULL(user->getNickname(), *it));
				break;
			case err_inviteonlychan:
			{
				srv->sendReply(userfd, ERR_INVITEONLYCHAN(user->getNickname(), *it));
				break;
			}
			default:
			{
				std::string msg = ":" + user->getMask() + " JOIN " + *it;
				channel->sendToAllMembers(msg);
				if (channel->isTopicSet())
					srv->sendReply(userfd, RPL_TOPIC(user->getNickname(), *it, channel->getTopic()));
				char symbol = (channel->secretMode() ? '@' : '=');
				srv->sendReply(userfd, RPL_NAMREPLY(user->getNickname(), symbol, *it, channel->namesStr(user)));
				srv->sendReply(userfd, RPL_ENDOFNAMES(user->getNickname(), *it));
				break;
			}
		}
	}
}
