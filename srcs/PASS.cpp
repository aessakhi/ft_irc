/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PASS.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aessakhi <aessakhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 14:41:19 by aessakhi          #+#    #+#             */
/*   Updated: 2023/02/16 15:34:49 by aessakhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandList.hpp"

void	pass(Server *srv, int &userfd, Command &cmd)
{
	std::string user = "New User";
	std::string cmdName = "PASS";

	if (cmd.getParamList().empty())
		srv->sendReply(userfd, ERR_NEEDMOREPARAMS(user, cmdName));
	if (cmd.getParamList()[0].compare(srv->getpwd()) != 0)
	{
		srv->sendReply(userfd, ERR_PASSWDMISMATCH(user));
		/* Might need to kill connection to user here */
	}
	if (cmd.getParamList()[0].compare(srv->getpwd()) == 0)
	{
		srv->getUser(userfd)->setPwd(true);
		std::cout << "Pwd is Ok" << std::endl;
	}
}
