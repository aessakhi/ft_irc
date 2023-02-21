/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NICK.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aessakhi <aessakhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 15:56:39 by aessakhi          #+#    #+#             */
/*   Updated: 2023/02/21 15:10:51 by aessakhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandList.hpp"

void	nick(Server *srv, int &userfd, Command &cmd)
{
	std::string	client;

	client = "";
	if (cmd.getParamList().empty())
		srv->sendReply(userfd, ERR_NONICKNAMEGIVEN(client));
	else if (srv->getUserbyNickname(cmd.getParamList()[0]) != NULL)
		srv->sendReply(userfd, ERR_NICKNAMEINUSE(client, cmd.getParamList()[0]));
	else
	{
		/* Need to check here if there are disallowed characters */
		std::cout << "Nick func" << std::endl;
		srv->getUser(userfd)->setNickname(cmd.getParamList()[0]);
		srv->getUser(userfd)->setNick(true);
		if (srv->getUser(userfd)->checkAuth() == true && srv->getUser(userfd)->getAuth() == false)
		{
			srv->getUser(userfd)->setAuth(true);
			srv->sendReply(userfd, RPL_WELCOME(srv->getUser(userfd)->getNickname()));
		}
	}
}
