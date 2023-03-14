#include "main.hpp"

std::string	cpt_memb(Channel *channel)
{
	size_t cpt = 0;
	for (; cpt < channel->getUsers().size(); cpt++);

	std::stringstream ss;
	ss << cpt;
	return ss.str();
}

void	list_all(Server *srv, int &userfd, Command &cmd, User *user)
{
	std::map<std::string, Channel *>::iterator it;
	(void)cmd;
	
	it = srv->getChannelMap()->begin();
	for (; it != srv->getChannelMap()->end(); it++)
		if (!it->second->secretMode())
			srv->sendReply(userfd, RPL_LIST(user->getNickname(), it->first,\
				cpt_memb(it->second), it->second->getTopic()));
}

void	list_select(Server *srv, int &userfd, Command &cmd, User *user)
{
	std::vector<std::string> vec = actual_split(cmd.getParam(0), ",#");
	std::map<std::string, Channel *>::iterator it;

	it = srv->getChannelMap()->begin();
	for (; it != srv->getChannelMap()->end(); it++) //EITHER LOOP FOR VEC.SIZE() OR FCT TO CHECK AND MATCH
		if (!it->second->secretMode())
			srv->sendReply(userfd, RPL_LIST(user->getNickname(), it->first,\
				cpt_memb(it->second), it->second->getTopic()));
}

void	list(Server *srv, int &userfd, Command &cmd)
{
	User * user = srv->getUser(userfd);

	srv->sendReply(userfd, RPL_LISTSTART(user->getNickname()));

	if (cmd.paramNumber() == 0)
		list_all(srv, userfd, cmd, user);
	else if (cmd.getParam(0).at(0) == '#')
		list_select(srv, userfd, cmd, user);

	srv->sendReply(userfd, RPL_LISTEND(user->getNickname()));
}