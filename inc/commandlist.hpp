#ifndef COMMANDLIST_HPP
# define COMMANDLIST_HPP

#include "main.hpp"

class Server;

void	away(Server *srv, int &userfd, Command &cmd);
void	cap(Server *srv, int &userfd, Command &cmd);
void	info(Server *srv, int &userfd, Command &cmd);
void	invite(Server *srv, int &userfd, Command &cmd);
void	join(Server *srv, int &userfd, Command &cmd);
void	kick(Server *srv, int &userfd, Command &cmd);
void	kill(Server *srv, int &userfd, Command &cmd);
void	list(Server *srv, int &userfd, Command &cmd);
void	mode(Server *srv, int &userfd, Command &cmd);
void	names(Server *srv, int &userfd, Command &cmd);
void	nick(Server *srv, int &userfd, Command &cmd);
void	notice(Server *srv, int &userfd, Command &cmd);
void	oper(Server *srv, int &userfd, Command &cmd);
void	part(Server *srv, int &userfd, Command &cmd);
void	pass(Server *srv, int &userfd, Command &cmd);
void	ping(Server *srv, int &userfd, Command &cmd);
void	privmsg(Server *srv, int &userfd, Command &cmd);
void	quit(Server *srv, int &userfd, Command &cmd);
void	srv_time(Server *srv, int &userfd, Command &cmd);
void	topic(Server *srv, int &userfd, Command &cmd);
void	user(Server *srv, int &userfd, Command &cmd);
void	version(Server *srv, int &userfd, Command &cmd);
void	wallops(Server * srv, int & userfd, Command & cmd);
void	who(Server *srv, int &userfd, Command &cmd);
void	whois(Server *srv, int &userfd, Command &cmd);
void	whowas(Server *srv, int &userfd, Command &cmd);
void	lusers(Server *srv, int &userfd, Command &cmd);

#endif
