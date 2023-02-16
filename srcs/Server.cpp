/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aessakhi <aessakhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 21:34:26 by aessakhi          #+#    #+#             */
/*   Updated: 2023/02/16 16:32:10 by aessakhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "codes.hpp"
#include "CommandList.hpp"

Server::Server(char *port, char *password): _port(std::string(port)), _pwd(std::string(password)), _listenfd(0), _epollfd(0)
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
		std::cerr << "Socket error" << std::endl;
		close(sockfd);
		exit(-1);
	}

	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &yes, sizeof yes) == -1)
	{
		std::cerr << "setsockopt" << std::endl;
		exit(-1);
	}

	if (fcntl(sockfd, F_SETFL, O_NONBLOCK) < 0)
	{
		std::cerr << "Socket error" << std::endl;
		close(sockfd);
		exit(-1);
	}

	srv_address.sin_family = AF_INET;
	srv_address.sin_port = htons(atoi(this->_port.c_str()));
	srv_address.sin_addr.s_addr = INADDR_ANY;
	memset(srv_address.sin_zero, 0, sizeof srv_address.sin_zero);

	if ((bind(sockfd, (struct sockaddr *)&srv_address, sizeof srv_address)) < 0)
	{
		std::cerr << "Bind error" << std::endl;
		close(sockfd);
		exit(-1);
	}

	if ((listen(sockfd, 10)) < 0)
	{
		std::cerr << "Listen error" << std::endl;
		close(sockfd);
		exit(-1);
	}
	this->_listenfd = sockfd;
}

void	Server::_create_epoll()
{
	struct epoll_event	ev;

	if ((this->_epollfd = epoll_create1(0)) == -1)
	{
		std::cerr << "epoll error" << std::endl;
		exit(-1);
	}
	memset(&ev, 0, sizeof(struct epoll_event));
	ev.events = EPOLLIN;
	ev.data.fd = this->_listenfd;
	if (epoll_ctl(this->_epollfd, EPOLL_CTL_ADD, this->_listenfd, &ev) == -1)
	{
		std::cerr << "epoll error" << std::endl;
		exit(-1);
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
		std::cerr << "accept error" << std::endl;
		exit(-1);
	}
	/* User will need to be destroyed if authenticate fails or when the connection is closed */
	this->_UserList[new_fd] = new User(new_fd);
	memset(&ev, 0, sizeof(struct epoll_event));
	ev.events = EPOLLIN;
	ev.data.fd = new_fd;
	if (epoll_ctl(this->_epollfd, EPOLL_CTL_ADD, new_fd, &ev) == -1)
	{
		std::cerr << "epoll error";
		exit(-1);
	}
}

void	Server::_execCmds(std::vector<Command> &cmds, int userfd)
{
	(void)userfd;
	for (std::vector<Command>::iterator it = cmds.begin(); it != cmds.end(); it++)
	{
		std::map<std::string, void(*)(Server *, int &, Command &)>::const_iterator it_map;
		it_map = this->_cmdMap.find(it->getCmd());
		if (it_map != this->_cmdMap.end())
			this->_cmdMap[it->getCmd()](this, userfd, *it);
		else
			std::cout << "Ignore cmd" << std::endl;
	}
}

void	Server::sendReply(int fd, std::string s)
{
	s += "\r\n";
	send(fd, s.data(), s.size(), MSG_NOSIGNAL);
	std::cout << "Sending " << s.size() << "bytes" << std::endl;
	std::cout << ">> " << s;
}

void	Server::_receivemessage(struct epoll_event event)
{
	char buf[RECV_BUFFER_SIZE];
	ssize_t ret;
	std::string msg;

	memset(buf, 0, RECV_BUFFER_SIZE);
	ret = recv(event.data.fd, buf, RECV_BUFFER_SIZE, 0);
	if (ret == -1)
	{
		std::cerr << "recv error" << std::endl;
		return ;
	}
	buf[ret] = 0;
	this->_buffers[event.data.fd].append(buf);
	/* Need to change the way the packets are received (In order the process a command, you have to first aggregate de received packets in order to rebuild it)*/
	std::vector<std::string>	cmds;
	/* Need to split the msg using \r\n placing the cmds in a vector. The first command vector will be used for authentication. If it doesn't respect the pre-requisites (PASS, NICK, USER), remove the user from epollfd and the User map */
	cmds = ft_split(&this->_buffers[event.data.fd], "\r\n");
	buf[0] = 0;
	for (std::vector<std::string>::const_iterator it = cmds.begin(); it != cmds.end(); it++)
		std::cout << *it << std::endl;
	std::cout << "-------------------------------" << std::endl;
	std::vector<Command>	cmd_vector;
	for (std::vector<std::string>::const_iterator it = cmds.begin(); it != cmds.end(); it++)
		splitCmds(&cmd_vector, *it);
	this->_execCmds(cmd_vector, event.data.fd);
	if (this->_UserList[event.data.fd]->getAuth() == false)
	{
		std::string s("User");
		//irssi needs to receive these numerical replies to confirm the connection. Need to add the expected details of the reply messages.
		this->sendReply(event.data.fd, RPL_WELCOME(s));
		// _reply(event.data.fd, RPL_YOURHOST(s));
		// _reply(event.data.fd, RPL_CREATED(s, t));
		// _reply(event.data.fd, RPL_MYINFO(s, ".", "."));
		this->_UserList[event.data.fd]->setAuth(true);
	}
	/* _removeUserfromServer(event.data.fd); */
}

void	Server::_removeUserfromServer(int userfd)
{
	if (this->getUser(userfd) == 0)
	{
		std::cerr << "Invalid user" << std::endl;
		return ;
	}
	this->_UserList.erase(userfd);
	if (epoll_ctl(this->_epollfd, EPOLL_CTL_DEL, userfd, NULL) == -1)
	{
		std::cerr << "epoll_ctl error" << std::endl;
		return ;
	}
	if (close(userfd) == -1)
	{
		std::cerr << "close error" << std::endl;
		return ;
	}
}

void	Server::_initCmdMap()
{
	this->_cmdMap["CAP"] = &cap;
	this->_cmdMap["PASS"] = &pass;
	this->_cmdMap["NICK"] = &nick;
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
			std::cerr << "epoll error" << std::endl;
			exit(-1);
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

std::string	Server::getpwd() const
{
	return (this->_pwd);
}

std::string	Server::getport() const
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
	for (std::map<int , User *>::const_iterator	it; it != this->_UserList.end(); it++)
	{
		if (nickname.compare(it->second->getNickname()) == 0)
			return (it->second);
	}
	return (NULL);
}

