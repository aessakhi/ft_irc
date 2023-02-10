/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldesnoye <ldesnoye@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 21:33:17 by aessakhi          #+#    #+#             */
/*   Updated: 2023/02/10 18:43:09 by ldesnoye         ###   ########.fr       */
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

	/* Returns a linked list of addrinfo structs that can be used to attempt to open a socket.
	Returns NULL if getaddrinfo() fails. (this should change) */
	struct addrinfo *_getaddrinfo_attempt() const;
	
	/* On every addrinfo in the given list, attempts to : open a socket, setsockopt (?), bind it to the address.
	Sets _listen_fd to the first possible fd on which it succeeded.
	Returns 0 on success, or -1 if an error occured. */
	int _bind_attempt(struct addrinfo * servinfo);

	/* Socket on which the server listens for new incoming connection requests */
	int	_listen_fd;

public:

	/* Returns the fd capable of accepting new connections. */
	int get_listening_fd() const;

	/* Initializes and opens _listen_fd for connection requests. */
	int begin_listening();

	/* Adds a pollfd to the vector _user_fds with the given fd and event mask POLLIN. */
	void	pollfd_push_back(int fd);

	Server(char * port, char * password);

	~Server();
};

#endif
