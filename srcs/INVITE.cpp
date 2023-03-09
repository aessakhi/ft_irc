#include "main.hpp"

void	invite(Server *srv, int &userfd, Command &cmd)
{
	User * fromuser = srv->getUser(userfd);
	std::string fromname = fromuser->getNickname();

	if (cmd.paramNumber() <= 1)
	{
		srv->sendReply(userfd, ERR_NEEDMOREPARAMS(fromname, cmd.getCmd()));
		return ;
	}

	std::string toname = cmd.getParam(0);
	User * touser = srv->getUserbyNickname(toname);
	std::string channelname = cmd.getParam(1);
	Channel * channel = srv->getChannel(channelname);

	if (touser == NULL)
	{
		srv->sendReply(userfd, ERR_NOSUCHNICK(fromname, toname));
		return ;
	}

	if (channel == NULL)
	{
		srv->sendReply(userfd, ERR_NOSUCHCHANNEL(fromname, channelname));
		return ;
	}

	if (!channel->isMember(fromuser))
	{
		srv->sendReply(userfd, ERR_NOTONCHANNEL(fromname, channelname));
		return ;
	}
	
	if (channel->inviteMode() && !channel->isOp(fromuser))
	{
		srv->sendReply(userfd, ERR_CHANOPRIVSNEEDED(fromname, channelname));
		return ;
	}

	if (channel->isMember(touser))
	{
		srv->sendReply(userfd, ERR_USERONCHANNEL(fromname, toname, channelname));
		return ;
	}
	
	srv->sendReply(userfd, RPL_INVITING(fromname, toname, channelname));
	std::string invite = ":" + fromuser->getMask() + " INVITE " + toname + " " + channelname;
	srv->sendReply(touser->getFd(), invite);
	channel->addInvite(touser);
}