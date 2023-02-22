#include "Server.hpp"

Server::Server(const std::string & name, const std::string & port, const std::string & password): _name(name), _port(port), _pwd(password), _listenfd(0), _epollfd(0)
{}

Server::~Server(){}

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
	
	/* User will need to be destroyed if authenticate fails or when the connection is closed */
	this->_UserList[new_fd] = new User(new_fd);
	memset(&ev, 0, sizeof(struct epoll_event));
	/* Need to recheck the flags to use, EPOLLIN is alright, but other flags might be useful */
	ev.events = EPOLLIN | EPOLLRDHUP;
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
		std::map<std::string, void(*)(Server *, int &, Command &)>::const_iterator it_map;
		it_map = this->_cmdMap.find(it->getCmd());
		if (it_map != this->_cmdMap.end())
		{
			printFcall(it->getCmd());
			this->_cmdMap[toupper(it->getCmd())](this, userfd, *it);
		}
		else
		{
			std::cout << YLW << "Ignore cmd" << RESET << std::endl;
		}
	}
}

void	Server::sendReply(int fd, std::string s)
{
	printReply(s);
	s += "\r\n";
	send(fd, s.data(), s.size(), MSG_NOSIGNAL);
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
	printRecv(buf, ret);

	this->_buffers[event.data.fd].append(buf, ret);

	std::vector<std::string>	cmds = split(&this->_buffers[event.data.fd], "\r\n");
	std::vector<Command>	cmd_vector;

	for (std::vector<std::string>::const_iterator it = cmds.begin(); it != cmds.end(); it++)
		cmd_vector.push_back(splitCmd(*it));
	
	for (size_t i = 0; i < cmd_vector.size(); i++)
		printSep(cmd_vector[i]);
		
	this->_execCmds(cmd_vector, event.data.fd);
	/* _removeUserfromServer(event.data.fd); */
}

void	Server::_removeUserfromServer(int userfd)
{
	if (this->getUser(userfd) == 0)
		return printError("Invalid user");
	this->_UserList.erase(userfd);
	if (epoll_ctl(this->_epollfd, EPOLL_CTL_DEL, userfd, NULL) == -1)
	{
		throw EpollCtlException();
	}
	if (close(userfd) == -1)
	{
		throw FdCloseException();
	}
}

void	Server::_initCmdMap()
{
	this->_cmdMap["CAP"] = &cap;
	this->_cmdMap["PASS"] = &pass;
	this->_cmdMap["NICK"] = &nick;
	this->_cmdMap["USER"] = &user;
	this->_cmdMap["PING"] = &ping;
	this->_cmdMap["PRIVMSG"] = &privmsg;
}

void	Server::init()
{
	struct	epoll_event ep_event[50];
	int		nfds;

	this->_initCmdMap();

	this->_createsocket();

	this->_create_epoll();

	while (1)
	{
		nfds = epoll_wait(this->_epollfd, ep_event, 50, 3000);
		if (nfds == -1)
		{
			throw EpollWaitException();
		}
		for (int i = 0; i < nfds; i++)
		{
			if (ep_event[i].data.fd == this->_listenfd)
				this->_acceptnewUser();
			else
				this->_receivemessage(ep_event[i]);
		}
	}
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