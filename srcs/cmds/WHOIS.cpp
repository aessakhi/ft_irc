#include "main.hpp"

std::string		time_idle(time_t start)
{
	std::string res;
	std::stringstream s;
	time_t rawtime;
	double sec;

	time(&rawtime);
	sec = difftime(rawtime, start);

	s << sec;
	res = s.str();
	return res;
}

void	whois(Server *srv, int &userfd, Command &cmd)
{
	User * user = srv->getUser(userfd);

	if (cmd.paramNumber() == 0)
	{
		srv->sendReply(userfd, ERR_NEEDMOREPARAMS(user->getNickname(), cmd.getCmd()));
		return;
	}

	std::string nickname;

	if (cmd.paramNumber() == 1)
		nickname = cmd.getParam(0);
	else
		nickname = cmd.getParam(1);

	User * target = srv->getUserbyNickname(nickname);

	if (target == NULL)
	{
		srv->sendReply(userfd, ERR_NOSUCHNICK(user->getNickname(), nickname));
		return;
	}

	std::stringstream creatime;
	creatime << target->getCreatime();

	srv->sendReply(userfd, RPL_WHOISUSER(user->getNickname(), nickname, target->getUsername(), target->getHostname(), target->getRealname()));
	srv->sendReply(userfd, RPL_WHOISIDLE(user->getNickname(), nickname, time_idle(target->getIdletime()), creatime.str()));
	if (target->isOperator())
		srv->sendReply(userfd, RPL_WHOISOPERATOR(user->getNickname(), nickname));
	if (target->isAway())
		srv->sendReply(userfd, RPL_AWAY(user->getNickname(), nickname, target->getAwayMessage()));
	srv->sendReply(userfd, RPL_ENDOFWHOIS(user->getNickname(), nickname));
}

/*
	possible replies to add:

    ~RPL_WHOISSERVER (312)
    RPL_WHOISCHANNELS (319)
    RPL_WHOISSPECIAL (320)
    RPL_WHOISACTUALLY (338)
    RPL_WHOISHOST (378)
    RPL_WHOISMODES (379)
    ~RPL_WHOISSECURE (671)
*/