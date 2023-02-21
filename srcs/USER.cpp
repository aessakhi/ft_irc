#include "main.hpp"

void	user(Server *srv, int &userfd, Command &cmd)
{
	std::string	client;

	client = "";
	if (cmd.getParam().empty())
	{
		if (srv->getUser(userfd)->getNick() == true)
			srv->sendReply(userfd, ERR_NEEDMOREPARAMS(srv->getUser(userfd)->getNickname(), cmd.getCmd()));
		else
			srv->sendReply(userfd, ERR_NEEDMOREPARAMS(client, cmd.getCmd()));
	}
	else
	{
		std::vector<std::string> name;
		std::string	realname;

		name = split(cmd.getParam(), " ");
		srv->getUser(userfd)->setUsername(name[0]);
		if (!cmd.getParamList().empty())
		{
			name = cmd.getParamList();
			for (std::vector<std::string>::const_iterator it = name.begin(); it != name.end(); it++)
			{
				realname = realname + *it;
				if (it + 1 != name.end())
					realname = realname + " ";
			}
		}
		srv->getUser(userfd)->setRealname(realname);
		srv->getUser(userfd)->setUser(true);
		if (srv->getUser(userfd)->checkAuth() == true && srv->getUser(userfd)->getAuth() == false)
		{
			srv->getUser(userfd)->setAuth(true);
			srv->sendReply(userfd, RPL_WELCOME(srv->getUser(userfd)->getNickname()));
		}
	}
}
