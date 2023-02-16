/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   NICK.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aessakhi <aessakhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 15:56:39 by aessakhi          #+#    #+#             */
/*   Updated: 2023/02/16 16:17:05 by aessakhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandList.hpp"

void	nick(Server *srv, int &userfd, Command &cmd)
{
	std::string	client;

	client = "New user";
	if (cmd.getParamList().empty())
		srv->sendReply(userfd, ERR_NONICKNAMEGIVEN(client));
	/* Need to add the check against the user list */
	else
	{
		srv->getUser(userfd)->setNickname(cmd.getParamList()[0]);
		std::cout << "Nickname succesfully added" << std::endl;
	}
}
