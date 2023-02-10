/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldesnoye <ldesnoye@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 21:34:26 by aessakhi          #+#    #+#             */
/*   Updated: 2023/02/10 17:57:45 by ldesnoye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(char * port, char * password): _port(std::string(port)), _password(std::string(password)), _listen_fd(0)
{}

struct addrinfo *Server::_getaddrinfo_attempt()
{
	struct addrinfo hints;
	struct addrinfo *servinfo;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	
	if (getaddrinfo(NULL, _port.c_str(), &hints, &servinfo))
		return NULL;

	return servinfo;
}

int Server::_bind_attempt(struct addrinfo * servinfo)
{
	int sockfd;
	int yes = 1;
	struct addrinfo *p;
	
	for (p = servinfo; p != NULL; p = p->ai_next)
	{
		sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		
		if (sockfd == -1)
		{
			continue;
		}
		
		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
		{
			std::cerr << "setsockopt error" << std::endl;
			close(sockfd);
			return -1;
		}
		
		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1)
		{
			std::cerr << "Bind error" << std::endl;
			close(sockfd);
			continue;
		}
		
		break;
	}
	
	if (p == NULL)
	{
		std::cerr << "Failed to bind" << std::endl;
		return -1;
	}
	
	_listen_fd = sockfd;
	
	return 0;
}

int Server::begin_listening()
{
	struct addrinfo *servinfo = _getaddrinfo_attempt();
	if (!servinfo)
	{
		return -1;
	}

	if (_bind_attempt(servinfo))
	{
		freeaddrinfo(servinfo);
		return -1;
	}

	freeaddrinfo(servinfo);

	// Maybe useless ?
	fcntl(_listen_fd, F_SETFD, O_NONBLOCK);
	
	if (listen(_listen_fd, 10) == -1)
	{
		std::cerr << "listen: error" << std::endl;
		return -1;
	}

	return 0;
}

Server::~Server()
{
	if (_listen_fd)
		close(_listen_fd);
}