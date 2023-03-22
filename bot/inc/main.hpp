#ifndef MAIN_HPP
#define MAIN_HPP

#include <string>
#include <stdexcept>
#include <iostream>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <vector>
#include <fcntl.h>
#include <sys/epoll.h>
#include <sstream>
#include <map>

#include "colors.hpp"
#include "exceptions.hpp"

#define RECV_BUFFER_SIZE 512
#define EPOLL_EVENTS_SIZE 10

class Bot;
class BotCommand;
class BotModes;
class Command;

#include "Bot.hpp"
#include "BotCommand.hpp"
#include "BotModes.hpp"
#include "Command.hpp"

/* -------------------- utils.cpp -------------------- */

void			printDebug(std::string s);
void			printCmdBuf(std::vector<Command> v);
void			printError(std::string s);
void			printReply(std::string s);
void			printRecv(char * s, size_t n);
std::string		no_crlf(std::string s);
void			printRecv(std::string s, size_t n);
std::string		toupper(std::string s);
std::vector<std::string> split(std::string str, std::string delimiter);
std::string		itoa(int n);

#endif