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
#include <sys/stat.h>
#include <ctime>
#include <cmath>
#include <list>

/* CLASSES */
class Channel;
class Command;
class Mode;
class Server;
class User;
class UserMask;

/* OTHER FILES */
#include "codes.hpp"
#include "colors.hpp"
#include "commandlist.hpp"
#include "exceptions.hpp"

/* CLASS HEADERS */
#include "Channel.hpp"
#include "Command.hpp"
#include "Mode.hpp"
#include "Server.hpp"
#include "User.hpp"
#include "UserMask.hpp"


#define RECV_BUFFER_SIZE 512

#define SHOW(s) std::cout << #s << " : [" << s << "]" << std::endl;



/* ---------- utils.cpp ---------- */

/*	Basic check for input arguments.
	Returns 0 if arguments are formatted correctly, -1 otherwise. */
int program_arguments_check(int argc, char **argv);

/* Splits str on delimiter.
Erases the beginning of str if there is a \\r\\n in str. */
std::vector<std::string> split(std::string *str, std::string delimiter);

/* Splits str on delimiter. */
std::vector<std::string> split(std::string str, std::string delimiter);

/* Returns a Command created from s.
s should be of format "prefix command command-parameters\\r\\n".
If there is a prefix, s begins with ':'.
command-parameters can be terminated by a message containing spaces, in which case it begins by ':'. */
Command	splitCmd(std::string s);

/* Returns true if s1 and s2 are similar when interpreting wildcards */
bool	wildcompare(std::string s1, std::string s2);

/* Returns true if s1 and s2 are similar when interpreting wildcards */
bool	wildcompare(const char * s1, const char * s2);

/* Returns true if s1 and s2 are similar when interpreting wildcards on s1 only */
bool	sided_wildcompare(std::string s1, std::string s2);

/* Returns true if s1 and s2 are similar when interpreting wildcards on s1 only */
bool	sided_wildcompare(const char * s1, const char * s2);

/* Returns a copy of s without the trailing \\r\\n */
std::string	no_crlf(std::string s);

/* Returns the uppercase version of s. */
std::string toupper(std::string s);

/* Returns true if s is of format *!*\@* where * is at least one char. */
bool isMask(std::string s);

/* ---------- print.cpp ---------- */

/* Prints a std::vector\<std::string> element by element. */
void	printVect(std::ostream & out, std::vector<std::string> vect);

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

/* Print vect on std::cout in green with elements visually separated. */
void	printSep(std::vector<std::string> vect);

/* ---------- MODE.cpp ---------- */

void	_usermode(Server *srv, int &userfd, Command &cmd);
void	_channelmode(Server *srv, int &userfd, Command &cmd);

#endif
