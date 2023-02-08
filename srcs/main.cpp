/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aessakhi <aessakhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 21:24:29 by aessakhi          #+#    #+#             */
/*   Updated: 2023/02/08 16:07:27 by aessakhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
#include "../include/Server.hpp"

int main(int argc, char **argv)
{
	//argv[0]: port
	//Need to get the port as int
	//argv[1]: password
	//Will be used in the Server constructor
	if (argc != 3)
	{
		std::cerr << "Error: Not enough arguments. Usage: ./ircsserv <port> <password>" << std::endl;
		return (-1);
	}

	int port;
	//Need check of argv[1] > isdigit
	std::istringstream(argv[1]) >> port;
	/* Server ircserv(port, argv[2]); */
	int sockfd;
	struct addrinfo hints, *servinfo, *p;
	int rv;
	/* int	numbytes; */
	struct sockaddr_storage their_addr;
	socklen_t sin_size;
	/* char buf[MAXBUFLEN]; */
	/* socklen_t addr_len; */
	int new_fd;
	int yes=1;
	char s[INET_ADDRSTRLEN];

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	
	if ((rv = getaddrinfo(NULL, argv[1], &hints, &servinfo)) != 0)
	{
		std::cerr << "getaddrinfo error" << std::endl;
		return (-1);
	}
	

	for (p = servinfo; p != NULL; p = p->ai_next)
	{
		if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
		{
			std::cerr << "socket error" << std::endl;
			continue;
		}
		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
		{
			std::cerr << "setsockopt error" << std::endl;
			exit(-1);
		}
		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1)
		{
			close(sockfd);
			std::cerr << "Bind error" << std::endl;
			continue;
		}

		struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
		void *addr = &(ipv4->sin_addr);

		std::cout << inet_ntop(p->ai_family, addr, s, sizeof s) << std::endl;
		freeaddrinfo(servinfo);
		break;
	}

	if (p == NULL)
	{
		std::cerr << "Failed to bind" << std::endl;
		exit(-1);
	}

	if (listen(sockfd, 10) == -1)
	{
		std::cerr << "listen: error" << std::endl;
		exit(-1);
	}

	while(1)
	{
		sin_size = sizeof their_addr;
		new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
		if (new_fd != -1)
		{
			std::cout << "Accepted connection" << std::endl;
			close(new_fd);
		}
		else
		{
			std::cerr << "Error" << std::endl;
			exit(-1);
		}
	}
}
