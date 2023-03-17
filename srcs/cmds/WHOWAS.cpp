#include "main.hpp"

void	whowas(Server *srv, int &userfd, Command &cmd)
{
	User * user = srv->getUser(userfd);

	if (cmd.paramNumber() == 0)
	{
		srv->sendReply(userfd, ERR_NEEDMOREPARAMS(user->getNickname(), cmd.getCmd()));
		return;
	}

	std::string target = cmd.getParam(0);
	std::vector<History> hist(srv->getHistory());
	bool	target_found = false;
	for (size_t i = 0; i < hist.size(); i++)
		if (hist[i].getNickname() == target)
		{
			target_found = true;
			srv->sendReply(userfd, RPL_WHOWASUSER(user->getNickname(), target, hist[i].getUsername(), hist[i].getHostname(), hist[i].getRealname()));
		}
	if (!target_found)
		srv->sendReply(userfd, ERR_NOSUCHNICK(user->getNickname(), target));
	srv->sendReply(userfd, RPL_ENDOFWHOWAS(user->getNickname(), target));
}