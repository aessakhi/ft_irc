/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aessakhi <aessakhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 21:34:26 by aessakhi          #+#    #+#             */
/*   Updated: 2023/02/11 16:21:27 by aessakhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(char *port, char *password): _port(std::string(port)), _password(std::string(password)), _sockfd(0)
{}

Server::~Server(){}

void	Server::_createsocket()
{
	struct sockaddr_in	srv_address;
	int		sockfd;

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

	this->_sockfd = sockfd;

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
	_createsocket();
}
