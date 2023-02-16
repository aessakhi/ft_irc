/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aessakhi <aessakhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/15 16:11:32 by aessakhi          #+#    #+#             */
/*   Updated: 2023/02/16 13:04:34 by aessakhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.hpp"
#include "Command.hpp"

Command::Command(std::string name, std::string prefix, std::vector<std::string> param_list): _cmd_name(name), _prefix(prefix), _params(param_list)
{}

Command::~Command()
{}

std::string	Command::getCmd() const
{
	return (this->_cmd_name);
}

std::string	Command::getPrefix() const
{
	return (this->_prefix);
}

std::vector<std::string> Command::getParamList() const
{
	return (this->_params);
}
