#include "main.hpp"

void	ping(Server *srv, int &userfd, Command &cmd)
{
	User * user = srv->getUser(userfd);

	// Number of args check
	if (cmd.paramNumber() == 0)
	{
		if (user->getNick() == true)
			srv->sendReply(userfd, ERR_NEEDMOREPARAMS(user->getNickname(), cmd.getCmd()));
		return ;
	}

	// PONG
	std::vector<std::string> param_list = cmd.getParamList();
	param_list.insert(param_list.begin(), srv->getName());
	Command reply("PONG", param_list, "");
	srv->sendReply(userfd, reply.getStr());
}