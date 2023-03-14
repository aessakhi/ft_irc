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

	std::string Nickname = cmd.getParam(0);
	User * target = srv->getUserbyNickname(Nickname);

	std::stringstream creatime;
	creatime << target->getCreatime();

	if (cmd.paramNumber() != 1 || !target)
		srv->sendReply(userfd, ERR_NOSUCHNICK(user->getNickname(), Nickname));
	else
	{
		srv->sendReply(userfd, RPL_WHOISUSER(user->getNickname(), Nickname, target->getUsername(), target->getHostname(), target->getRealname()));
		srv->sendReply(userfd, RPL_WHOISIDLE(user->getNickname(), Nickname, time_idle(target->getIdletime()), creatime.str()));
		if (target->isAway())
			srv->sendReply(userfd, RPL_AWAY(user->getNickname(), Nickname, target->getAwayMessage()));
	}	
	srv->sendReply(userfd, RPL_ENDOFWHOIS(user->getNickname(), Nickname));
}