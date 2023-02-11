/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aessakhi <aessakhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 21:33:17 by aessakhi          #+#    #+#             */
/*   Updated: 2023/02/11 20:00:55 by aessakhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include "main.hpp"

//Need to stock the user list -> ? , channel list -> Map easier? pair<std::string, Channel*> ?

class Server
{
private:

	Server();

	/* Port used for connecting to the server */
	std::string	_port;
	/* Password requested for connecting to the server */
	std::string	_password;

	/* Socket on which the server listens for new incoming connection requests */
	int	_listenfd;

	int	_epollfd;

	/* _createsocket will be called by init to create and bind the socket */
	void	_createsocket(int sockfd);

public:

	Server(char *port, char *password);

	~Server();

	/* Functions */
	void	init();

	/* Member accessors */
	std::string	getpwd() const;
	std::string	getport() const;

};

#endif
