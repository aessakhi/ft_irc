/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldesnoye <ldesnoye@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 21:33:17 by aessakhi          #+#    #+#             */
/*   Updated: 2023/02/10 17:57:37 by ldesnoye         ###   ########.fr       */
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

	/* Vector containing the pollfd structs that are passed as arguments to poll() */
	std::vector<struct pollfd>	_user_fds;

	/* Returns a linked list of addrinfo structs that can be used to attempt to open a socket.
	Returns NULL if getaddrinfo() fails. (this should change) */
	struct addrinfo *_getaddrinfo_attempt();
	
	/* On every addrinfo in the given list, attempts to : open a socket, setsockopt (?), bind it to the address.
	Sets _listen_fd to the first possible fd on which it succeeded.
	Returns 0 on success, or -1 if an error occured. */
	int _bind_attempt(struct addrinfo * servinfo);

public:

	/* Port used for connecting to the server */
	std::string		_port;
	/* Password requested for connecting to the server */
	std::string		_password;

	/* Socket on which the server listens for new incoming connection requests */
	int				_listen_fd;

	int begin_listening();

	Server(char * port, char * password);

	~Server();
};

std::ostream & operator<<(std::ostream& out, const Server & dt);

#endif
