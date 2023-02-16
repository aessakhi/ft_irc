/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CAP.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aessakhi <aessakhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 13:53:57 by aessakhi          #+#    #+#             */
/*   Updated: 2023/02/16 14:31:54 by aessakhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CommandList.hpp"

void	cap(Server *srv, int &userfd, Command &cmd)
{
	(void)srv;
	(void)userfd;
	(void)cmd;
	std::cout << "CAP ignored" << std::endl;
}
