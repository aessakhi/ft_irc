/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_arg_check.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ldesnoye <ldesnoye@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/10 17:53:06 by ldesnoye          #+#    #+#             */
/*   Updated: 2023/02/10 17:55:39 by ldesnoye         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"

/* Returns true only if the given string could be a port. */
static bool _port_is_digit(char *port)
{
	for (size_t i = 0; port[i]; i++)
	{
		if (!isdigit(port[i]))
			return false;
	}
	return (port[0] != 0);
}

/*	Basic check for input arguments.
	Returns 0 if arguments are formatted correctly, -1 otherwise. */
int program_arguments_check(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cerr << "Error: Wrong number of arguments. Usage: ./ircsserv <port> <password>" << std::endl;
		return -1;
	}

	if (!_port_is_digit(argv[1]))
	{
		std::cerr << "Error: Port should be a numeric value" << std::endl;
		return -1;
	}

	if (!argv[2][0])
	{
		std::cerr << "Error: Password cannot be empty." << std::endl;
		return -1;
	}
	
	return 0;
}