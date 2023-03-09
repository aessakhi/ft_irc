#include "main.hpp"

void	kick(Server *srv, int &userfd, Command &cmd)
{
	User * fromuser = srv->getUser(userfd);
	std::string fromname = fromuser->getNickname();

	if (cmd.paramNumber() <= 1)
	{
		srv->sendReply(userfd, ERR_NEEDMOREPARAMS(fromname, cmd.getCmd()));
		return ;
	}

	std::string channelname = cmd.getParam(0);
	Channel * channel = srv->getChannel(channelname);

	if (channel == NULL)
	{
		srv->sendReply(userfd, ERR_NOSUCHCHANNEL(fromname, channelname));
		return ;
	}

	std::string kickedname = cmd.getParam(1);
	User * kickeduser = srv->getUserbyNickname(kickedname);

	if (kickeduser == NULL)
	{
		srv->sendReply(userfd, ERR_NOSUCHNICK(fromname, kickedname));
		return ;
	}

	if (!channel->isMember(fromuser))
	{
		srv->sendReply(userfd, ERR_NOTONCHANNEL(fromname, channelname));
		return ;
	}

	if (!channel->isOp(fromuser))
	{
		srv->sendReply(userfd, ERR_CHANOPRIVSNEEDED(fromname, channelname));
		return ;
	}

	if (!channel->isMember(kickeduser))
	{
		srv->sendReply(userfd, ERR_USERNOTINCHANNEL(fromname, kickedname, channelname));
		return ;
	}

	std::string msg = ":" + fromuser->getMask() + " KICK " + channelname + " " + kickedname + " :" + cmd.getLastParam();
	channel->sendToAllMembers(msg);
	channel->kick(kickeduser);
}