/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aessakhi <aessakhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 21:33:17 by aessakhi          #+#    #+#             */
/*   Updated: 2023/02/14 16:37:49 by aessakhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include "main.hpp"
#include "User.hpp"
//Need to stock the user list -> pair<int, User*> , channel list -> Map easier? pair<std::string, Channel*> ?

class Server
{
private:

	Server();

	/* Port used for connecting to the server */
	std::string	_port;
	/* Password requested for connecting to the server */
	std::string	_pwd;

	/* Socket on which the server listens for new incoming connection requests */
	int	_listenfd;

	int	_epollfd;

	std::map<int, User *>	_UserList;

	std::map<int, std::string>	_buffers;
	/* epoll is way easier to use and navigate than poll, no need to add a vector with the different pollfd */
	void	_create_epoll();

	/* _createsocket will be called by init to create and bind the socket */
	void	_createsocket();

	void	_acceptnewUser();

	void	_receivemessage(struct epoll_event event);

	void	_removeUserfromServer(int fd);

public:

	Server(char *port, char *password);

	~Server();

	/* Functions */
	void	init();

	/* Member accessors */
	std::string	getpwd() const;
	std::string	getport() const;
	User	*getUser(int fd) const;

};

#endif
