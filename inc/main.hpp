#ifndef MAIN_HPP
# define MAIN_HPP

#include <iostream>
#include <sstream>
#include <cstring>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <vector>
#include <map>
#include <sys/epoll.h>
#include <signal.h>
#include <stdexcept>
#include <algorithm>

/* CLASSES */
class Channel;
class Command;
class Server;
class User;

/* OTHER FILES */
#include "codes.hpp"
#include "colors.hpp"
#include "commandlist.hpp"
#include "exceptions.hpp"

/* CLASS HEADERS */
#include "Channel.hpp"
#include "Command.hpp"
#include "Server.hpp"
#include "User.hpp"


#define RECV_BUFFER_SIZE 512

#define SHOW(s) std::cout << #s << " : [" << s << "]" << std::endl;



/* ---------- utils.cpp ---------- */



/*	Basic check for input arguments.
	Returns 0 if arguments are formatted correctly, -1 otherwise. */
int program_arguments_check(int argc, char **argv);

std::vector<std::string> split(std::string *str, std::string delimiter);
std::vector<std::string> split(std::string str, std::string delimiter);
void	splitCmds(std::vector<Command> *cmd_vector, std::string cmd);

/* Returns true if s1 and s2 are similar when interpreting wildcards */
bool	wildcompare(std::string s1, std::string s2);

/* Returns true if s1 and s2 are similar when interpreting wildcards */
bool	wildcompare(const char * s1, const char * s2);

/* Returns a copy of s without the trailing \\r\\n */
std::string	no_crlf(std::string s);

/* Returns the uppercase version of s. */
std::string toupper(std::string s);



/* ---------- print.cpp ---------- */



/* Prints a std::vector\<std::string> element by element. */
void	vectprint(std::ostream & out, std::vector<std::string> vect);

/* Print s on std::cerr in red. */
void	printError(std::string s);

/* Print sent replies on std::cout in blue. */
void	printReply(std::string s);

/* Print command function calls on std::cout in yellow. */
void	printFcall(std::string s);

/* Print n chars of received messages on std::cout in blue without trailing \\r\\n. */
void	printRecv(char * s, size_t n);

/* Print n char of received messages on std::cout in blue without trailing \\r\\n. */
void	printRecv(std::string s, size_t n);

/* Print cmd on std::cout in green detailing the different attributes. */
void	printSep(Command cmd);

#endif
