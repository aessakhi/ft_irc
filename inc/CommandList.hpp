/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CommandList.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aessakhi <aessakhi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/16 13:52:32 by aessakhi          #+#    #+#             */
/*   Updated: 2023/02/21 14:24:04 by aessakhi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMANDLIST_HPP
# define COMMANDLIST_HPP

#include "main.hpp"
#include "Server.hpp"
#include "codes.hpp"

class Server;

void	cap(Server *srv, int &userfd, Command &cmd);
void	pass(Server *srv, int &userfd, Command &cmd);
void	nick(Server *srv, int &userfd, Command &cmd);
void	user(Server *srv, int &userfd, Command &cmd);

#endif
