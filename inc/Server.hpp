/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aessakhi <aessakhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 21:33:17 by aessakhi          #+#    #+#             */
/*   Updated: 2023/02/11 16:21:01 by aessakhi         ###   ########.fr       */
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

	/* Vector containing the pollfd structs that are passed as arguments to poll().
	The first element is the fd on which clients request access, the others are fds for communication with users.*/
	std::vector<struct pollfd> _user_fds;

	/* _createsocket will be called by init to create and bind the socket */
	void	_createsocket();

	/* Socket on which the server listens for new incoming connection requests */
	int	_sockfd;

public:

	Server(char *port, char *password);

	~Server();

	void	init();
};

#endif
