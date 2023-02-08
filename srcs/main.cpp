/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aessakhi <aessakhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/02 21:24:29 by aessakhi          #+#    #+#             */
/*   Updated: 2023/02/02 22:16:16 by aessakhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <iostream>
#include <sstream>
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
	Server ircserv(port, argv[2]);
}
