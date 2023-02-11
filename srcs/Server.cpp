/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aessakhi <aessakhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 21:34:26 by aessakhi          #+#    #+#             */
/*   Updated: 2023/02/11 21:04:50 by aessakhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(char *port, char *password): _port(std::string(port)), _password(std::string(password)), _listenfd(0)
{}

Server::~Server(){}

void	Server::_createsocket(int sockfd)
{
	struct sockaddr_in	srv_address;

	sockfd = socket(PF_INET, SOCK_STREAM, 0);

	if (sockfd < 0)
	{
		std::cerr << "Socket error" << std::endl;
		close(sockfd);
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
}

void	Server::init()
{
	struct	epoll_event ev;
	struct	epoll_event ep_event[50];
	int		nfds;

	socklen_t			addr_length;
	struct sockaddr_in	client_addr;

	this->_createsocket(this->_listenfd);

	if ((this->_epollfd = epoll_create1(0)) == -1)
	{
		std::cerr << "epoll error" << std::endl;
		exit(-1);
	}
	ev.events = EPOLLIN;
	ev.data.fd = this->_listenfd;
	if (epoll_ctl(this->_epollfd, EPOLL_CTL_ADD, this->_listenfd, &ev) == -1)
	{
		std::cerr << "epoll error" << std::endl;
		exit(-1);
	}
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
			{
				std::cout << "Here" << std::endl;
				addr_length = sizeof(struct sockaddr_in);
				int new_fd;
				if ((new_fd = accept(this->_listenfd, (struct sockaddr*)&client_addr, &addr_length)) == -1)
				{
					std::cerr << "accept error" << std::endl;
					exit(-1);
				}
				ev.events = EPOLLIN;
				ev.data.fd = new_fd;
				if (epoll_ctl(this->_epollfd, EPOLL_CTL_ADD, new_fd, &ev) == -1)
				{
					std::cerr << "epoll error" << std::endl;
					exit(-1);
				}
			}
			else
			{
				
			}
		}
	}
}
