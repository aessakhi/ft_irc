#include "main.hpp"

void	topic(Server *srv, int &userfd, Command &cmd)
{
	User * user = srv->getUser(userfd);
	std::string client_name = user->getNickname();

	if (cmd.paramNumber() == 0)
	{
		srv->sendReply(userfd, ERR_NEEDMOREPARAMS(client_name, cmd.getCmd()));
		return ;
	}

	std::string channel_name = cmd.getParam(0);
	Channel * channel = srv->getChannel(channel_name);

	if (channel == NULL)
	{
		srv->sendReply(userfd, ERR_NOSUCHCHANNEL(client_name, channel_name));
		return ;
	}

	if (cmd.paramNumber() == 1)
	{
		// Reply with the topic
		std::string topic = channel->getTopic();
		if (topic.empty())
			srv->sendReply(userfd, RPL_NOTOPIC(client_name, channel_name));
		else
			srv->sendReply(userfd, RPL_TOPIC(client_name, channel_name, topic));
		return ;
	}

	// Change the topic

	if (channel->protectedTopicMode() && !channel->isOp(user))
	{
		srv->sendReply(userfd, ERR_CHANOPRIVSNEEDED(client_name, channel_name));
		return ;
	}

	std::string new_topic;
	if (cmd.paramNumber() == 2)
		new_topic = cmd.getLastParam();
	else
		new_topic = cmd.getParam(1);
	
	channel->setTopic(new_topic);
	channel->sendToAllMembers(":" + user->getMask() + " " + cmd.getStr());
	channel->up_topictime();
}
