/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aessakhi <aessakhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 21:33:17 by aessakhi          #+#    #+#             */
/*   Updated: 2023/02/02 22:15:30 by aessakhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include <string>

//Need to stock the user list -> ? , channel list -> Map easier? pair<std::string, Channel*> ?

class Server
{
private:
	int			_port;
	std::string	_password;

public:
	Server();

	Server(int port, std::string password);

	~Server();
};

#endif
