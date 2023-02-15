/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aessakhi <aessakhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/10 17:53:06 by ldesnoye          #+#    #+#             */
/*   Updated: 2023/02/15 16:42:59 by aessakhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"
#include "Command.hpp"

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

/* Might need to be tweaked a bit, might be an issue when a string has no trailing \r\n */
std::vector<std::string> ft_split(std::string *str, std::string delimiter)
{
	std::vector<std::string> result;

	size_t	end = 0;

	if (str->find(delimiter) == std::string::npos)
	{
		str->erase(std::string::npos);
		return (std::vector<std::string>());
	}
	while (str->find(delimiter, 0) != std::string::npos)
	{
		end = str->find(delimiter, 0);
		result.push_back(str->substr(0, end + delimiter.length()));
		str->erase(0, end + delimiter.length());
	}
	return (result);
}

void	splitCmds(std::vector<Command> *cmd_vector, std::string cmd)
{
	size_t						end	= 0;
	std::string					name;
	std::string					prefix;
	std::vector<std::string>	param_list;

	if ((end = cmd.find(" ")) != std::string::npos)
	{
		name = cmd.substr(0, end);
		cmd.erase(0, end + 1);
		std::cout << "Name = " << name << std::endl;
	}
	if ((end = cmd.find(":")) != std::string::npos)
	{
		prefix = cmd.substr(0, end);
		cmd.erase(0, end + 1);
		std::cout << "Prefix = " << prefix << std::endl;
	}
	while ((end = cmd.find(" ")) != std::string::npos)
	{
		param_list.push_back(cmd.substr(0, end + 1));
		cmd.erase(0, end + 1);
	}
	if (!cmd.empty())
		param_list.push_back(cmd);
	for (std::vector<std::string>::const_iterator it = param_list.begin(); it != param_list.end(); it++)
	{
		std::cout << *it << std::endl;
	}
	cmd_vector->push_back(Command(name, prefix, param_list));
}
