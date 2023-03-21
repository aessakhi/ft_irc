#include "main.hpp"

static void	_privmsg_channel(Server *srv, int &userfd, Command &cmd, std::string &client, std::string target)
{
	Channel	*channel = srv->getChannel(target);
	User	*sender = srv->getUser(userfd);
	if (channel == NULL) /* Channel doesn't exist */
	{
		srv->sendReply(userfd, ERR_NOSUCHCHANNEL(client, target));
		return ;
	}
	/* Need to check if the sender is banned AND NOT covered by a ban exception + WILL SILENTLY FAIL */
	/* Also need to check if the channel is in moderated mode */
	if (channel->noExternalMessagesMode())
	{
		if (!channel->isMember(sender))
		{
			srv->sendReply(userfd, ":" + srv->getHostname() + " " + ERR_CANNOTSENDTOCHAN(client, target));
			return ;
		}
	}
	if (channel->isBanned(sender) && !channel->isBanExcept(sender))
		return ;
	if (channel->moderatedMode() && !channel->isOp(sender) && !channel->isVoiced(sender))
	{
		srv->sendReply(userfd, ":" + srv->getHostname() + " " + ERR_CANNOTSENDTOCHAN(client, target));
		return ;
	}
	channel->sendToAllMembers(":" + sender->getMask() + " PRIVMSG " + target + " :" + cmd.getLastParam(), userfd);
}

static void	_privmsg_user(Server *srv, int &userfd, Command &cmd, std::string &client, std::string target)
{
	int targetfd = srv->getUserfd(target);
	if (targetfd == -1)
	{
		srv->sendReply(userfd, ERR_NOSUCHNICK(client, target));
		return ;
	}
	if (srv->getUser(targetfd)->isAway())
		srv->sendReply(userfd, ":" + srv->getHostname() + " " + RPL_AWAY(client, srv->getUser(targetfd)->getNickname(), srv->getUser(targetfd)->getAwayMessage()));
	srv->sendReply(targetfd, ":" + srv->getUser(userfd)->getMask() + " PRIVMSG " + target + " :" + cmd.getLastParam());
}

void	privmsg(Server *srv, int &userfd, Command &cmd)
{
	std::string client;

	client = srv->getUser(userfd)->getNickname();
	if (cmd.paramNumber() == 0 || cmd.getParam(0).empty())
	{
		srv->sendReply(userfd, ERR_NORECIPIENT(client));
		return ;
	}
	if (cmd.paramNumber() == 1 || cmd.getParam(1).empty())
	{
		srv->sendReply(userfd, ERR_NOTEXTTOSEND(client));
		return ;
	}
	
	std::vector<std::string> targets = actual_split(cmd.getParam(0), ",");
	std::vector<std::string>::const_iterator it = targets.begin();
	std::vector<std::string>::const_iterator ite = targets.end();
	for (; it != ite; it++)
	{
		if (it->empty())
			continue;
		if (it->at(0) == '#')
			_privmsg_channel(srv, userfd, cmd, client, *it);
		else
			_privmsg_user(srv, userfd, cmd, client, *it);
	}
}


/* The PRIVMSG command is used to send private messages between users, as well as to send messages to channels. <target> is the nickname of a client or the name of a channel.

If <target> is a channel name and the client is banned and not covered by a ban exception, the message will not be delivered and the command will silently fail. Channels with the moderated mode active may block messages from certain users. Other channel modes may affect the delivery of the message or cause the message to be modified before delivery, and these modes are defined by the server software and configuration being used.

If a message cannot be delivered to a channel, the server SHOULD respond with an ERR_CANNOTSENDTOCHAN (404) numeric to let the user know that this message could not be delivered.

If <target> is a channel name, it may be prefixed with one or more channel membership prefix character (@, +, etc) and the message will be delivered only to the members of that channel with the given or higher status in the channel. Servers that support this feature will list the prefixes which this is supported for in the STATUSMSG RPL_ISUPPORT parameter, and this SHOULD NOT be attempted by clients unless the prefix has been advertised in this token.

If <target> is a user and that user has been set as away, the server may reply with an RPL_AWAY (301) numeric and the command will continue.

The PRIVMSG message is sent from the server to client to deliver a message to that client. The <source> of the message represents the user or server that sent the message, and the <target> represents the target of that PRIVMSG (which may be the client, a channel, etc).

When the PRIVMSG message is sent from a server to a client and <target> starts with a dollar character ('$', 0x24), the message is a broadcast sent to all clients on one or multiple servers.

											Numeric Replies

										ERR_NOSUCHNICK (401)
										ERR_NOSUCHSERVER (402)
										ERR_CANNOTSENDTOCHAN (404)
										ERR_TOOMANYTARGETS (407)
										ERR_NORECIPIENT (411)
										ERR_NOTEXTTOSEND (412)
										ERR_NOTOPLEVEL (413)
										ERR_WILDTOPLEVEL (414)
										RPL_AWAY (301)
 */
