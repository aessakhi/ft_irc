#include "Server.hpp"

Server::Server(const std::string & name, const std::string & port, const std::string & password, const std::string hostname, const std::string & op_pwd): _name(name), _port(port), _pwd(password),_hostname(hostname), _listenfd(0), _epollfd(0), _op_pwd(op_pwd)
{}

Server::~Server()
{
	// delete channels
	std::map<std::string, Channel *>::iterator it_chan = _channelMap.begin();
	std::map<std::string, Channel *>::iterator ite_chan = _channelMap.end();
	for (; it_chan != ite_chan; it_chan++)
	{
		delete it_chan->second;
	}

	// delete users
	std::map<int, User *>::iterator it_user = _UserList.begin();
	std::map<int, User *>::iterator ite_user = _UserList.end();
	for (; it_user != ite_user; it_user++)
	{
		delete it_user->second;

		if (epoll_ctl(this->_epollfd, EPOLL_CTL_DEL, it_user->first, NULL) == -1)
		{
			throw EpollCtlException();
		}
		if (close(it_user->first) == -1)
		{
			throw FdCloseException();
		}
	}

	// delete _listenfd and epollfd
	if (_epollfd)
	{
		if (epoll_ctl(_epollfd, EPOLL_CTL_DEL, _listenfd, NULL) == -1)
		{
			throw EpollCtlException();
		}
		if (close(_epollfd) == -1)
		{
			throw FdCloseException();
		}
	}
	if (_listenfd)
	{
		if (close(_listenfd) == -1)
		{
			throw FdCloseException();
		}
	}
}

void	Server::_createsocket()
{
	struct	sockaddr_in	srv_address;
	int		sockfd;
	int		yes = 1;

	sockfd = socket(PF_INET, SOCK_STREAM, 0);

	if (sockfd < 0)
	{
		close(sockfd);
		throw SocketCreationException();
	}

	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &yes, sizeof yes) == -1)
	{
		close(sockfd);
		throw SocketCreationException();
	}

	if (fcntl(sockfd, F_SETFL, O_NONBLOCK) < 0)
	{
		close(sockfd);
		throw SocketCreationException();
	}

	srv_address.sin_family = AF_INET;
	srv_address.sin_port = htons(atoi(this->_port.c_str()));
	srv_address.sin_addr.s_addr = INADDR_ANY;
	memset(srv_address.sin_zero, 0, sizeof srv_address.sin_zero);

	if ((bind(sockfd, (struct sockaddr *)&srv_address, sizeof srv_address)) < 0)
	{
		close(sockfd);
		throw SocketCreationException();
	}

	if ((listen(sockfd, 10)) < 0)
	{
		close(sockfd);
		throw SocketCreationException();
	}
	this->_listenfd = sockfd;
}

void	Server::_create_epoll()
{
	struct epoll_event	ev;

	if ((this->_epollfd = epoll_create1(0)) == -1)
	{
		throw EpollCreationException();
	}
	
	memset(&ev, 0, sizeof(struct epoll_event));
	ev.events = EPOLLIN;
	ev.data.fd = this->_listenfd;
	
	if (epoll_ctl(this->_epollfd, EPOLL_CTL_ADD, this->_listenfd, &ev) == -1)
	{
		throw EpollCtlException();
	}
}

void	Server::_acceptnewUser()
{
	int					new_fd;
	socklen_t			addr_length;
	struct sockaddr_in	client_addr;
	struct	epoll_event	ev;

	addr_length = sizeof(struct sockaddr_in);
	if ((new_fd = accept(this->_listenfd, (struct sockaddr *)&client_addr, &addr_length)) == -1)
	{
		throw NewUserAcceptException();
	}
	
	if (fcntl(new_fd, F_SETFL, O_NONBLOCK) < 0)
	{
		close(new_fd);
		throw SocketCreationException();
	}

	/* User will need to be destroyed if authenticate fails or when the connection is closed */
	this->_UserList[new_fd] = new User(new_fd, inet_ntoa(client_addr.sin_addr));
	memset(&ev, 0, sizeof(struct epoll_event));
	/* Need to recheck the flags to use, EPOLLIN is alright, but other flags might be useful */
	ev.events = EPOLLIN | EPOLLOUT | EPOLLRDHUP;
	ev.data.fd = new_fd;
	if (epoll_ctl(this->_epollfd, EPOLL_CTL_ADD, new_fd, &ev) == -1)
	{
		throw EpollCtlException();
	}
}

void	Server::_execCmds(std::vector<Command> &cmds, int userfd)
{
	for (std::vector<Command>::iterator it = cmds.begin(); it != cmds.end(); it++)
	{
		if (isAuthcmd((*it).getCmd()) || getUser(userfd)->getAuth())
		{
			std::map<std::string, void(*)(Server *, int &, Command &)>::const_iterator it_map;
			it_map = this->_cmdMap.find(it->getCmd());
			if (it_map != this->_cmdMap.end())
			{
				if (it->getCmd() != "PING" && getUser(userfd) != NULL)
					getUser(userfd)->updateIdletime();
				this->_cmdMap[toupper(it->getCmd())](this, userfd, *it);
			}
			else
				this->sendReply(userfd, ERR_UNKNOWNCOMMAND(this->getUser(userfd)->getNickname(), it->getCmd()));
		}
	}
}

void	Server::sendReply(int fd, std::string s)
{
	printReply(s);
	s += "\r\n";
	this->getUser(fd)->getUserbuffer().append(s);
}

std::vector<Command>	Server::_parseBuffers(struct epoll_event event)
{
	std::vector<std::string>	cmds = split(&_buffers[event.data.fd], "\r\n");
	std::vector<Command>	cmd_vector;

	for (std::vector<std::string>::const_iterator it = cmds.begin(); it != cmds.end(); it++)
		cmd_vector.push_back(splitCmd(*it));
	
	return cmd_vector;
}

void	Server::_receivemessage(struct epoll_event event)
{
	char buf[RECV_BUFFER_SIZE];
	ssize_t ret;

	ret = recv(event.data.fd, buf, RECV_BUFFER_SIZE, 0);
	if (ret == -1)
	{
		return printError("recv error");
	}
	if (ret == 0)
		return ;
	printRecv(buf, ret);

	this->_buffers[event.data.fd].append(buf, ret);
}

void	Server::_removeUserfromServer(int userfd)
{
	User * user = this->getUser(userfd);
	if (user == 0)
		return printError("Invalid user");
	err_codes	err;
	for (std::map<std::string, Channel *>::const_iterator it = _channelMap.begin(); it != _channelMap.end(); it++)
	{
		err = (*it).second->part(user);
		if (err == err_noerror)
			(*it).second->sendToAllMembers(":" + user->getMask() + " PART " + (*it).first + " Connection error");
	}
	_history.push_back(*user);
	this->_UserList.erase(userfd);
	if (epoll_ctl(this->_epollfd, EPOLL_CTL_DEL, userfd, NULL) == -1)
	{
		throw EpollCtlException();
	}
	delete user;
	if (close(userfd) == -1)
	{
		throw FdCloseException();
	}
}

void	Server::_initCmdMap()
{
	this->_cmdMap["AWAY"] = &away;
	this->_cmdMap["CAP"] = &cap;
	this->_cmdMap["INFO"] = &info;
	this->_cmdMap["INVITE"] = &invite;
	this->_cmdMap["JOIN"] = &join;
	this->_cmdMap["KICK"] = &kick;
	this->_cmdMap["KILL"] = &kill;
	this->_cmdMap["LIST"] = &list;
	this->_cmdMap["MODE"] = &mode;
	this->_cmdMap["NAMES"] = &names;
	this->_cmdMap["NICK"] = &nick;
	this->_cmdMap["NOTICE"] = &notice;
	this->_cmdMap["OPER"] = &oper;
	this->_cmdMap["PART"] = &part;
	this->_cmdMap["PASS"] = &pass;
	this->_cmdMap["PING"] = &ping;
	this->_cmdMap["PRIVMSG"] = &privmsg;
	this->_cmdMap["QUIT"] = &quit;
	this->_cmdMap["TIME"] = &srv_time;
	this->_cmdMap["TOPIC"] = &topic;
	this->_cmdMap["USER"] = &user;
	this->_cmdMap["VERSION"] = &version;
	this->_cmdMap["WALLOPS"] = &wallops;
	this->_cmdMap["WHO"] = &who;
	this->_cmdMap["WHOIS"] = &whois;
	this->_cmdMap["WHOWAS"] = &whowas;
	this->_cmdMap["LUSERS"] = &lusers;
}

void	Server::epoll_loop()
{
	struct	epoll_event ep_event[50];
	int nfds;
	std::vector<Command> cmd_vector;

	nfds = epoll_wait(this->_epollfd, ep_event, 50, 0);
	if (nfds == -1)
	{
		throw EpollWaitException();
	}
	for (int i = 0; i < nfds; i++)
	{
		if (ep_event[i].events & EPOLLIN)
		{
			if (ep_event[i].data.fd == this->_listenfd)
			{
				_acceptnewUser();
			}
			else
			{
				_receivemessage(ep_event[i]);
				cmd_vector = _parseBuffers(ep_event[i]);
				_execCmds(cmd_vector, ep_event[i].data.fd);
			}
		}
		if (ep_event[i].events & EPOLLOUT)
		{
			if (this->getUser(ep_event[i].data.fd))
				if (!this->getUser(ep_event[i].data.fd)->getUserbuffer().empty())
				{
					std::string s = this->getUser(ep_event[i].data.fd)->getUserbuffer();
					send(ep_event[i].data.fd, s.data(), s.size(), MSG_NOSIGNAL);
					this->getUser(ep_event[i].data.fd)->getUserbuffer().clear();
				}
		}
		if (ep_event[i].events & EPOLLRDHUP)
		{
			_removeUserfromServer(ep_event[i].data.fd);
		}
	}
}

void	Server::init()
{
	this->_initCmdMap();

	this->_createsocket();

	this->_create_epoll();

	time(&this->_creatime);
}

const std::string &	Server::getName() const
{
	return (this->_name);
}

const std::string &	Server::getpwd() const
{
	return (this->_pwd);
}

const std::string &	Server::getport() const
{
	return (this->_port);
}

const std::string	Server::getHostname() const
{
	return (this->_hostname);
}

User	*Server::getUser(int fd) const
{
	if (this->_UserList.find(fd) != this->_UserList.end())
		return (this->_UserList.find(fd)->second);
	return (NULL);
}

User	*Server::getUserbyNickname(const std::string nickname) const
{
	for (std::map<int , User *>::const_iterator	it = this->_UserList.begin(); it != this->_UserList.end(); it++)
	{
		if (nickname == it->second->getNickname())
			return (it->second);
	}
	return (NULL);
}

int		Server::getUserfd(const std::string nickname) const
{
	for (std::map<int , User *>::const_iterator	it = this->_UserList.begin(); it != this->_UserList.end(); it++)
	{
		if (nickname == it->second->getNickname())
			return (it->first);
	}
	return (-1);
}

Channel * Server::getChannel(const std::string & channel_name)
{
	std::map<std::string, Channel *>::const_iterator it = _channelMap.begin();
	std::map<std::string, Channel *>::const_iterator ite = _channelMap.end();

	for (; it != ite; it++)
	{

		if (channel_name == it->first)
			return it->second;
	}
	return NULL;
}

std::map<std::string, Channel *> *Server::getChannelMap()
{
	return &(this->_channelMap);
}

std::map<int, User *> Server::getUserMap() const
{
	return _UserList;
}

time_t Server::getCreatime() const
{ 
	return (this->_creatime);
}

bool Server::check_oper_password(std::string str) const
{
	return !(_op_pwd.compare(str));
}

std::vector<History> &Server::getHistory()
{
	return _history;
}
