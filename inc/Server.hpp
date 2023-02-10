/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldesnoye <ldesnoye@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 21:33:17 by aessakhi          #+#    #+#             */
/*   Updated: 2023/02/10 15:10:45 by ldesnoye         ###   ########.fr       */
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
	std::string		_port;
	/* Password requested for connecting to the server */
	std::string		_password;
	
	/* Socket on which the server listens for new incoming connection requests */
	int							_listen_fd;
	/* Vector containing the pollfd structs that are passed as arguments to poll() */
	std::vector<struct pollfd>	_user_fds;

public:

	Server(std::string port, std::string password);

	~Server();
};

#endif
