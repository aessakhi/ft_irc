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

#include "exceptions.hpp"
#include "Command.hpp"
#include "codes.hpp"

#define RECV_BUFFER_SIZE 512

#define SHOW(s) std::cout << #s << " : [" << s << "]" << std::endl;

// main_arg_check.cpp
int program_arguments_check(int argc, char **argv);
std::vector<std::string> split(std::string *str, std::string delimiter);
std::vector<std::string> split(std::string str, std::string delimiter);
void	splitCmds(std::vector<Command> *cmd_vector, std::string cmd);

#endif
