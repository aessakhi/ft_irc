#include "main.hpp"

static std::string	cpt_memb(Channel *channel)
{
	size_t cpt = 0;
	for (; cpt < channel->getUsers().size(); cpt++);

	std::stringstream ss;
	ss << cpt;
	return ss.str();
}

static size_t	number_of_memb(Channel *channel)
{
	size_t cpt = 0;
	for (; cpt < channel->getUsers().size(); cpt++);

	return cpt;
}

static bool	more_or_less(size_t num_user, char sign, size_t cpt_req)
{
	if (sign == '>')
		return (num_user > cpt_req);
	if (sign == '<')
		return (num_user < cpt_req);
	return false;
}

static double	uptime_min(time_t start)
{
	time_t rawtime;
	time(&rawtime);
	double sec = difftime(rawtime, start);

	double uptime = 0;
	for (; sec >= 60; sec -= 60)
		uptime += 1;
	return uptime;
}

void	list_all(Server *srv, int &userfd, Command &cmd, User *user)
{
	std::map<std::string, Channel *>::iterator it;
	(void)cmd;
	
	it = srv->getChannelMap()->begin();
	for (; it != srv->getChannelMap()->end(); it++)
		if (!it->second->secretMode())
		{
			std::cout << it->first << " is " << it->second->secretMode() << std::endl;
			srv->sendReply(userfd, RPL_LIST(user->getNickname(), it->first,\
				cpt_memb(it->second), it->second->getTopic()));
		}
}

void	list_select(Server *srv, int &userfd, Command &cmd, User *user)
{
	std::vector<std::string> vec = actual_split(cmd.getParam(0), ",");
	std::map<std::string, Channel *>::iterator it;
	std::vector<std::string>::iterator vec_it;

	for (vec_it = vec.begin(); vec_it != vec.end(); vec_it++)
		for (it = srv->getChannelMap()->begin(); it != srv->getChannelMap()->end(); it++)
			if (!it->second->secretMode() && *vec_it == it->first)
				srv->sendReply(userfd, RPL_LIST(user->getNickname(), it->first,\
					cpt_memb(it->second), it->second->getTopic()));
}

void	list_all_users(Server *srv, int &userfd, Command &cmd, User *user)
{
	std::map<std::string, Channel *>::iterator it;

	std::string res = cmd.getParam(0).substr(1, 15);
	if (res.find_first_not_of("0123456789") != std::string::npos)
		return ;

	srv->sendReply(userfd, RPL_LISTSTART(user->getNickname()));

	size_t cpt = atoi(res.c_str());
	it = srv->getChannelMap()->begin();
	for (; it != srv->getChannelMap()->end(); it++)
		if (!it->second->secretMode() && more_or_less(number_of_memb(it->second), cmd.getParam(0).at(0), cpt))
			srv->sendReply(userfd, RPL_LIST(user->getNickname(), it->first, cpt_memb(it->second), it->second->getTopic()));

	srv->sendReply(userfd, RPL_LISTEND(user->getNickname()));
}

void	list_chan_time(Server *srv, int &userfd, Command &cmd, User *user)
{
	std::map<std::string, Channel *>::iterator it;

	std::string res = cmd.getParam(0).substr(2, 16);
	if (res.find_first_not_of("0123456789") != std::string::npos)
		return ;
	srv->sendReply(userfd, RPL_LISTSTART(user->getNickname()));
	
	double cpt = atof(res.c_str());
	it = srv->getChannelMap()->begin();
	for (; it != srv->getChannelMap()->end(); it++)
		if (!it->second->secretMode() && more_or_less(uptime_min(it->second->getCreatime()), cmd.getParam(0).at(1), cpt))
			srv->sendReply(userfd, RPL_LIST(user->getNickname(), it->first, cpt_memb(it->second), it->second->getTopic()));

	srv->sendReply(userfd, RPL_LISTEND(user->getNickname()));
}

void	list_topic_time(Server *srv, int &userfd, Command &cmd, User *user)
{
	std::map<std::string, Channel *>::iterator it;

	std::string res = cmd.getParam(0).substr(2, 16);
	if (res.find_first_not_of("0123456789") != std::string::npos)
		return ;
	srv->sendReply(userfd, RPL_LISTSTART(user->getNickname()));
	
	double cpt = atof(res.c_str());
	it = srv->getChannelMap()->begin();
	for (; it != srv->getChannelMap()->end(); it++)
		if (!it->second->secretMode() && more_or_less(uptime_min(it->second->getTopictime()), cmd.getParam(0).at(1), cpt))
			srv->sendReply(userfd, RPL_LIST(user->getNickname(), it->first, cpt_memb(it->second), it->second->getTopic()));

	srv->sendReply(userfd, RPL_LISTEND(user->getNickname()));
}

void	list(Server *srv, int &userfd, Command &cmd)
{
	User * user = srv->getUser(userfd);

	if (cmd.paramNumber() == 0)
	{
		srv->sendReply(userfd, RPL_LISTSTART(user->getNickname()));
		list_all(srv, userfd, cmd, user);
		srv->sendReply(userfd, RPL_LISTEND(user->getNickname()));
	}
	else if (cmd.getParam(0).at(0) == '#')
	{
		srv->sendReply(userfd, RPL_LISTSTART(user->getNickname()));
		list_select(srv, userfd, cmd, user);
		srv->sendReply(userfd, RPL_LISTEND(user->getNickname()));
	}
	else if (cmd.getParam(0).at(0) == '>' || cmd.getParam(0).at(0) == '<')
		list_all_users(srv, userfd, cmd, user);
	else if (cmd.getParam(0).size() >= 2)
	{
		if (cmd.getParam(0).at(0) == 'C' && (cmd.getParam(0).at(1) == '>' || cmd.getParam(0).at(1) == '<' ))
			list_chan_time(srv,userfd, cmd, user);
		else if (cmd.getParam(0).at(0) == 'T' && (cmd.getParam(0).at(1) == '>' || cmd.getParam(0).at(1) == '<' ))
			list_topic_time(srv,userfd, cmd, user);
	}
}