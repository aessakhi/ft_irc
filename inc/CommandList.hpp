#ifndef COMMANDLIST_HPP
# define COMMANDLIST_HPP

#include "main.hpp"
#include "Server.hpp"
#include "codes.hpp"

class Server;

void	cap(Server *srv, int &userfd, Command &cmd);
void	pass(Server *srv, int &userfd, Command &cmd);
void	nick(Server *srv, int &userfd, Command &cmd);
void	user(Server *srv, int &userfd, Command &cmd);

#endif
