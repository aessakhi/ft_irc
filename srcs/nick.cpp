/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aessakhi <aessakhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 15:56:39 by aessakhi          #+#    #+#             */
/*   Updated: 2023/02/16 16:08:55 by aessakhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandList.hpp"

void	nick(Server *srv, int &userfd, Command &cmd)
{
	if (cmd.getParamList().empty())
		srv->sendReply(userfd, "431 :No Nickname given");
	/* Need to add the check against the user list */
	else
	{
		srv->getUser(userfd)->setNickname(cmd.getParamList()[0]);
		std::cout << "Nickname succesfully added" << std::endl;
	}
}
