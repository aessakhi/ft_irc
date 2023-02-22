#include "main.hpp"

static void	privmsg_channel(Server *srv, int &userfd, Command &cmd, std::string &client)
{
	std::cout << "Target is a channel" << std::endl;
	(void)srv;
	(void)userfd;
	(void)cmd;
	(void)client;
}

static void	privmsg_user(Server *srv, int &userfd, Command &cmd, std::string &client)
{
	std::cout << "Target is a user" << std::endl;
	if (srv->getUserbyNickname(cmd.getParam(0)) == NULL)
	{
		srv->sendReply(userfd, ERR_NORECIPIENT(client));
		return ;
	}
}

void	privmsg(Server *srv, int &userfd, Command &cmd)
{
	std::string client;

	client = srv->getUser(userfd)->getNickname();
	if (cmd.getParam(0).empty())
	{
		srv->sendReply(userfd, ERR_NORECIPIENT(client));
		return ;
	}
	if (cmd.getParamList().size() == 1)
	{
		srv->sendReply(userfd, ERR_NOTEXTTOSEND(client));
		return ;
	}
	if (cmd.getParamList().size() > 2)
	{
		srv->sendReply(userfd, ERR_TOOMANYTARGETS(client, cmd.getParam(0)));
		return ;
	}
	if (cmd.getParam(0).find("#") != std::string::npos)
	{
		privmsg_channel(srv, userfd, cmd, client);
		return ;
	}
	else
	{
		privmsg_user(srv, userfd, cmd, client);
		return ;
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
