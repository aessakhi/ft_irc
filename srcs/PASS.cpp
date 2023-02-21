/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PASS.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aessakhi <aessakhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/21 13:41:07 by aessakhi          #+#    #+#             */
/*   Updated: 2023/02/21 15:29:20 by aessakhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandList.hpp"

void	pass(Server *srv, int &userfd, Command &cmd)
{
	std::string user = "New User";
	std::string cmdName = "PASS";

	if (cmd.getParamList().empty())
		srv->sendReply(userfd, ERR_NEEDMOREPARAMS(user, cmdName));
	else if (cmd.getParamList()[0].compare(srv->getpwd()) != 0)
	{
		srv->sendReply(userfd, ERR_PASSWDMISMATCH(user));
		/* Might need to kill connection to user here */
	}
	else if (cmd.getParamList()[0].compare(srv->getpwd()) == 0)
	{
		std::cout << "Pass func" << std::endl;
		srv->getUser(userfd)->setPwd(true);
		if (srv->getUser(userfd)->checkAuth() == true && srv->getUser(userfd)->getAuth() == false)
		{
			srv->getUser(userfd)->setAuth(true);
			srv->sendReply(userfd, RPL_WELCOME(srv->getUser(userfd)->getNickname()));
		}
	}
}
