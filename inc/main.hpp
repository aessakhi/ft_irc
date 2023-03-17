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
#include <utility>

/* CLASSES */

class Channel;
class Command;
class Mode;
class Server;
class User;
class History;

/* OTHER FILES */

#include "codes.hpp"
#include "colors.hpp"
#include "commandlist.hpp"
#include "exceptions.hpp"
#include "print.hpp"
#include "utils.hpp"

/* CLASS HEADERS */

#include "History.hpp"
#include "Channel.hpp"
#include "Command.hpp"
#include "Mode.hpp"
#include "Server.hpp"
#include "User.hpp"

#define RECV_BUFFER_SIZE 512

#define SHOW(s) std::cout << #s << " : [" << s << "]" << std::endl;

#endif
